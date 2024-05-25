#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <QObject>
#include <QThread>

class EventHandler : public QThread
{
    Q_OBJECT

private:
    int period;
    bool stopFlag;

public:
    EventHandler(int microSeconds);

    void changePeriod(int microSeconds);
    void start();
    void stop();
signals:
    void forwardSig();
    void startSig();
    void stopSig();
};

#endif // EVENTHANDLER_H