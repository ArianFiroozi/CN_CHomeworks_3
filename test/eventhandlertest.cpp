#include <iostream>
#include <stdio.h>
#include <QVector>
#include <QThread>
#include <unistd.h>

#include "./headers/eventHandler.h"
#include "./headers/cluster.h"
#include "./headers/pc.h"

using namespace std;

QString event_handler_with_mesh_cluster_static_init_correct()
{
    Mesh cluster(4, 4, IPv4("255.255.255.255", "20.0.0.0"));
    Packet myPack("hello world", MSG, IPV4, IPv4("255.255.255.255", "20.0.0.1"),
                  IPv4("255.255.255.255", "192.168.20.4"));

    EventHandler* eventHandler = new EventHandler(100);
    QThread* eventThread = new QThread();
    eventHandler->moveToThread(eventThread);

    PC sender(1, new IPv4("255.255.255.255", "192.168.20.1"), new Port(1));
    PC receiver(2, new IPv4("255.255.255.255", "192.168.20.4"), new Port(2));

    cluster.routers.last()->addPort(new Port(3));
    QString path("../resources/routingTables/manualMesh4x4/routingTable16.csv");
    cluster.routers.last()->routingTable.initFromFile(path, cluster.routers.last()->getPortWithID(3));

    QObject::connect(sender.port, &Port::getPacket,
                     cluster.routers[0], &Router::recievePacket);
    QObject::connect(cluster.routers.last()->getPortWithID(3), &Port::getPacket,
                     &receiver, &PC::recievePacket);

    for (Router* router : cluster.routers)
    {
        QObject::connect(eventHandler, &EventHandler::forwardSig,
                         router, &Router::forward);
    }

    // receiver.moveToThread(cluster.threads.last());
    eventThread->start();

    sender.sendPacket(myPack);

    emit eventHandler->startSig();

    usleep(100000);

    if (receiver.buffer[0].getString() != "hello world")
        return "message did not reach other pc in mesh!";
    return "";
}

QVector<QString> run_event_handler_tests()
{
    QVector<QString> errors;

    errors += event_handler_with_mesh_cluster_static_init_correct();

    return errors;
}
