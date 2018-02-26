#ifndef THREADDATA_H
#define THREADDATA_H
#include <QVector>
#include <QMutex>
#include <QReadWriteLock>


class Threaddata
{

public:
    static Threaddata *instance();
    QVector<double> xs;
    QVector<double> ys;
    QVector<double> ybuffer1;
    QVector<double> ybuffer2;
    QVector<double> ylockpoint;
    void Lock();
    void Unlock();
    int wpointer;
    int bufferPointNumber;
    int lastpoint;
    QReadWriteLock RWlock;
    int totPoint;
    int startPoint;
    int bufferPoint;
    int ori_ratio;
    int now_ratio;
    bool bufferFlag;
    bool connectFlag;
    bool recvFlag;
    bool stopFlag;
    bool lockpointFlag;
    double ChannelOnePos;
    double ChannelOneVdiv;
    void Init_xs_ys(int);
    void TimeDiv_change(int, int, int);
    void First_Init();
    void Init_buffer(int);
    void lockPointFunc();

private:
    Threaddata();
    ~Threaddata();
    QMutex Mutex;


};

#endif // THREADDATA_H
