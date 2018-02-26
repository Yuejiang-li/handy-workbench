#include "threaddata.h"

Threaddata::Threaddata()
{}

Threaddata::~Threaddata()
{}

Threaddata *Threaddata::instance()
{
    static Threaddata siglton;
    return &siglton;
}

void Threaddata::Lock()
{
    Mutex.lock();
}


void Threaddata::Unlock()
{
    Mutex.unlock();
}

void Threaddata::Init_xs_ys(int pointnumber)
{
    int j;
    double step_size = 10.0/(pointnumber-1);
    xs.resize(pointnumber);
    ys.resize(pointnumber);
    for (j = 0; j< pointnumber; j++)
    {
        xs[j] = -5 + step_size*j;
        ys[j] = ChannelOnePos;
    }
    if (pointnumber > 100)
    {
        wpointer = 0;
    }
}

void Threaddata::Init_buffer(int bufnumber)
{
    ybuffer1.resize(bufnumber);
    ybuffer2.resize(bufnumber);
    for (int i; i<bufnumber; i++)
    {
        ybuffer1[i] = 0;
        ybuffer2[i] = 0;
    }
}

void Threaddata::First_Init()
{
    wpointer = 0;
    totPoint = 11;
    ori_ratio = 1;
    now_ratio = 1;
    bufferPoint = 2*totPoint - 1;
    startPoint = (bufferPoint - 1)/4;
    bufferPointNumber = 0;
    lastpoint = 0;
    xs.reserve(501);
    ys.reserve(501);
    ybuffer1.reserve(1001);
    ybuffer2.reserve(1001);
    ylockpoint.reserve(1001);
    bufferFlag = true;
    connectFlag = false;
    recvFlag = false;
    stopFlag = false;
    lockpointFlag = false;
    ChannelOnePos = 0.0;
    ChannelOneVdiv = 1.0;
    Lock();

    Init_xs_ys(11);
    for (int i=0; i<bufferPoint; i++)
    {
        ybuffer1.append(0);
        ybuffer2.append(0);
    }

    Unlock();
}

void Threaddata::TimeDiv_change(int pointnumber, int ratio, int ori_r)
{
    int k;
    now_ratio = ratio;
    if (connectFlag)
    {
        totPoint = pointnumber;
        bufferPoint = 2*pointnumber -1;
        if (stopFlag)
        {
            Init_xs_ys(pointnumber);
            Init_buffer(2*pointnumber - 1);
            ori_ratio = now_ratio;
        }
        else
        {
            if (lockpointFlag)
            {
                int using_num;
                if (ori_ratio >= 10)
                {
                    using_num = ylockpoint.size() - 1;
                }
                else
                {
                    using_num = (ylockpoint.size() - 1)/2;
                }
                int plot_size = ratio * using_num / ori_ratio + 1;
                Init_xs_ys(plot_size);
                if (ratio > ori_ratio)
                {
                    for (k=0; k<ylockpoint.size(); k++)
                    {
                        ys[k] = ChannelOneVdiv * ylockpoint[k] + ChannelOnePos;
                    }
                }
                else
                {
                    int start_pointer = (ylockpoint.size() - plot_size)/2;
                    int end_pointer = start_pointer + plot_size - 1;
                    for (k = start_pointer; k<=end_pointer; k++)
                    {
                        ys[k-start_pointer] = ylockpoint[k];
                    }
                }
            }
        }
    }
}

void Threaddata::lockPointFunc()
{
    int i;
    if ((!stopFlag)&&(!lockpointFlag))
    {
        if (now_ratio >= 10)
        {
            ylockpoint.resize(totPoint);
            for (i=0; i<totPoint; i++)
            {
                ylockpoint[i] = ys[i];
            }
        }
        else
        {
            ylockpoint.resize(bufferPoint);
            if (bufferFlag)
            {
                for (i=0; i<bufferPoint; i++)
                {
                    ylockpoint[i] = ybuffer2[i];
                }
            }
            else
            {
                for (i=0; i<bufferPoint; i++)
                {
                    ylockpoint[i] = ybuffer1[i];
                }
            }
        }
        lockpointFlag = true;
    }
}
