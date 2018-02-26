#include "datarcvthread.h"
#include <QDebug>

DataRcvThread::DataRcvThread()
{
}

void DataRcvThread::run()
{
    TcpSocket = new QTcpSocket(this);
    connect(TcpSocket, SIGNAL(readyRead()),
            this, SLOT(readMessage()));
    TcpSocket->abort();
    TcpSocket->connectToHost("192.168.47.1", 8080);
    QByteArray connnectsuccessfultxt = "(connecte successful!)\n";
    //connnectsuccessfultxt.prepend(char(170));
    //connnectsuccessfultxt.prepend(char(170));

    TcpSocket->write(connnectsuccessfultxt);
    Threaddata::instance()->connectFlag = true;
    exec();
}

void DataRcvThread::startRunning()
{
    Threaddata::instance()->Lock();
    Threaddata::instance()->stopFlag = true;
    Threaddata::instance()->ori_ratio = Threaddata::instance()->now_ratio;
    Threaddata::instance()->lockpointFlag = false;
    Threaddata::instance()->Init_xs_ys(Threaddata::instance()->totPoint);
    Threaddata::instance()->Init_buffer(Threaddata::instance()->bufferPoint);
    QByteArray requestDataTxt = "(please send data!)\n";
    //requestDataTxt.prepend(char(0x04));
    //requestDataTxt.prepend(char(0x00));
    TcpSocket->write(requestDataTxt);
    TcpSocket->flush();
    Threaddata::instance()->Unlock();
}

void DataRcvThread::SlotTimeDivChangeMessage(QByteArray *Mes)
{
    TcpSocket->write(*Mes);
    TcpSocket->flush();
}

void DataRcvThread::readMessage()
{
    char recvtxt[1002];
    const int number = int(TcpSocket->read(recvtxt,1002));
    Threaddata::instance()->Lock();
    int pointNumber = Threaddata::instance()->totPoint;
    int bufpointNumber = Threaddata::instance()->bufferPoint;
    if (pointNumber < 100)
    {
        if (Threaddata::instance()->bufferFlag)
        {
            int i;
            if (number == bufpointNumber)
            {
                for (i = 0; i < bufpointNumber; i++)
                {
                    Threaddata::instance()->ybuffer1[i] = double(recvtxt[i] - 48);
                }
            }
        }
        else
        {
            int i;
            if (number == bufpointNumber)
            {
                for (i = 0; i < bufpointNumber; i++)
                {
                    Threaddata::instance()->ybuffer2[i] = double(recvtxt[i] - 48);
                }
            }
        }
        Threaddata::instance()->recvFlag = true;
    }
    else
    {
        if (Threaddata::instance()->bufferFlag)
        {
            Threaddata::instance()->bufferPointNumber = number;
            int i;
            if (number <= pointNumber)
            {
                for (i = 0; i < pointNumber; i++)
                {
                    Threaddata::instance()->ybuffer1[i] = double(recvtxt[i] - 48);
                }
            }
        }
        else
        {
            Threaddata::instance()->bufferPointNumber = number;
            int i;
            if (number <= pointNumber)
            {
                for (i = 0; i < pointNumber; i++)
                {
                    Threaddata::instance()->ybuffer2[i] = double(recvtxt[i] - 48);
                }
            }
        }
        Threaddata::instance()->recvFlag = true;
    }

    Threaddata::instance()->Unlock();
}

void DataRcvThread::sendRequestMessage()
{
    QByteArray requestDataTxt = "(please send data!)\n";
    //requestDataTxt.prepend(char(0x04));
    //requestDataTxt.prepend(char(0x00));
    TcpSocket->write(requestDataTxt);
    TcpSocket->flush();
}


void DataRcvThread::SlotSendSignalParameter(QByteArray *paraArray)
{
    TcpSocket->write(*paraArray);
    TcpSocket->flush();
}

void DataRcvThread::SlotStopSignal()
{
    Threaddata::instance()->Lock();
    Threaddata::instance()->stopFlag = false;
    QByteArray txt = "(stop sending data stream!)\n";
    //txt.prepend(char(0x05));
    //txt.prepend(char(0x00));
    TcpSocket->write(txt);
    TcpSocket->flush();
    Threaddata::instance()->lockPointFunc();
    Threaddata::instance()->Unlock();
}
