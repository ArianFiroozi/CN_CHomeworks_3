#include "./headers/router.h"

Router::Router(int _id, QThread *parent)
    : QThread{parent}
{
    id = _id;
}

void Router::recievePacket(Packet packet)
{
    // infinite buffer
    buffer.append(packet);
    cout<<"buffer"<<id<<"recieved smt!"<<endl;
}

void Router::forward()
{
    for (Packet packet:buffer)
        sendPacket(packet);
}

bool Router::sendPacket(Packet packet)
{
    IPv4 dest = packet.getDest();
    Route sendRoute = routingTable.findBestRoute(dest);

    if (!packet.getDest().includes(sendRoute.dest))
        return false;

    sendRoute.port->write(packet);
    return true;
}

void Router::addPort(Port* newPort)
{
    ports.append(newPort);
}

Port* Router::getPortWithID(int portID)
{
    for (Port* port : ports)
        if (port->id == portID)
            return port;
}


