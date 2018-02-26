#ifndef DATARCVTHREAD_H
#define DATARCVTHREAD_H

#include <QThread>
#include "threaddata.h"
#include <QTcpSocket>

class DataRcvThread:public QThread
{
    Q_OBJECT

public:
    DataRcvThread();

protected:
    void run();

private:
    QTcpSocket* TcpSocket;

private slots:
    void startRunning();
    void readMessage();
    void sendRequestMessage();
    void SlotSendSignalParameter(QByteArray*);
    void SlotStopSignal();
    void SlotTimeDivChangeMessage(QByteArray*);
};

#endif // DATARCVTHREAD_H
