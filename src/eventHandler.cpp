#include <headers/eventHandler.h>

#include <iostream>
#include <unistd.h>

using namespace std;

EventHandler::EventHandler(int microSeconds, double _stopTime)
{
    period = microSeconds;
    stopTime = _stopTime;
    QObject::connect(this, &EventHandler::stopSig, this, &EventHandler::stop);
    QObject::connect(this, &EventHandler::startSig, this, &EventHandler::start);
}

void EventHandler::changePeriod(int microSeconds)
{
    period = microSeconds;
}

void EventHandler::start()
{
    stopFlag = false;
    double time = 0;
    while (time < stopTime)
    {
        usleep(period);
        emit tick(++time);
    }
}

void EventHandler::stop()
{
    cout << "stopping!"<<endl;
    stopFlag = true;
}
