#include "plot.h"
#include "threaddata.h"
#include <qwt_plot_layout.h>
#include <qwt_plot_grid.h>
#include <qwt_point_data.h>
#include <QEvent>
#include <QDebug>

Plot::Plot(QWidget * parent):
    QwtPlot(parent)
{
    setFixedSize(800,600);
    setAutoReplot( false );
    Threaddata::instance()->First_Init();
    plotLayout()->setAlignCanvasToScales(true);
    setAxisTitle(QwtPlot::xBottom,"X");
    setAxisTitle(QwtPlot::yLeft,"Y");
    setAxisScale(QwtPlot::xBottom,-5,5,1);
    setAxisScale(QwtPlot::yLeft,-5.0,5.0,1);
    setCanvasBackground(Qt::black);

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setPen(Qt::gray,0.0,Qt::DotLine);
    grid->enableX(true);
    grid->enableY(true);
    grid->attach(this);

    d_origin = new QwtPlotMarker();
    d_origin->setLineStyle(QwtPlotMarker::Cross);
    d_origin->setValue(0.0, 0.0);
    d_origin->setLinePen(Qt::gray, 0.0, Qt::DashLine);
    d_origin->attach(this);

    channel_one_Marker = new QwtPlotMarker();
    channel_one_Marker->setLineStyle(QwtPlotMarker::HLine);
    channel_one_Marker->setYValue(0.0);
    channel_one_Marker->setLinePen(Qt::darkYellow, 0.0, Qt::SolidLine);
    channel_one_Marker->attach(this);

    d_curve = new QwtPlotCurve();
    d_curve->setStyle(QwtPlotCurve::Lines);
    d_curve->setCurveAttribute(QwtPlotCurve::Fitted, true);
    d_curve->setPen(Qt::yellow,0.0,Qt::SolidLine);
    d_curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    d_curve->setCurveAttribute(QwtPlotCurve::Fitted,true);;
    d_curve->setSamples(Threaddata::instance()->xs,Threaddata::instance()->ys);
    d_curve->attach(this);
}

Plot::~Plot()
{

}

void Plot::start()
{
    d_timerID = startTimer(20);
}

void Plot::replot()
{
//    Threaddata::instance()->Lock();

    d_curve->setSamples(Threaddata::instance()->xs,Threaddata::instance()->ys);
    QwtPlot::replot();

//    Threaddata::instance()->Unlock();

}

void Plot::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == d_timerID)
    {
        if (Threaddata::instance()->connectFlag)
        {
            if (Threaddata::instance()->recvFlag)
            {
                Threaddata::instance()->recvFlag = false;
                updateCurve();
            }
        }
    }
}

void Plot::updateCurve()
{
    Threaddata::instance()->Lock();

    int pointNumber = Threaddata::instance()->totPoint;
    int start = Threaddata::instance()->startPoint;
    int bufferPointNumber = Threaddata::instance()->bufferPointNumber;
    double pos = Threaddata::instance()->ChannelOnePos;
    double OneDiv = Threaddata::instance()->ChannelOneVdiv;

    if (pointNumber <= 100)
    {
        if (Threaddata::instance()->bufferFlag)
        {
            Threaddata::instance()->bufferFlag = false;
            //发送数据接收请求
            emit plotDataRequest();
            //将buffer1中的数据更换或添加至ys中;
            int i;
            for (i=start; i<start + pointNumber; i++)
            {
                Threaddata::instance()->ys[i-start] = OneDiv * Threaddata::instance()->ybuffer1[i] + pos;
            }
        }
        else
        {
            Threaddata::instance()->bufferFlag = true;
            emit plotDataRequest();
            int i;
            for (i=start; i<start + pointNumber; i++)
            {
                Threaddata::instance()->ys[i-start] = OneDiv * Threaddata::instance()->ybuffer2[i] + pos;
            }
        }
    }
    else
    {
        if (Threaddata::instance()->bufferFlag)
        {
            Threaddata::instance()->bufferFlag = false;
            emit plotDataRequest();
            int k = 0;
            int pointer = Threaddata::instance()->wpointer;
            while (k < bufferPointNumber)
            {
                Threaddata::instance()->ys[pointer] = OneDiv * Threaddata::instance()->ybuffer1[k] + pos;
                pointer++;
                if (pointer >100)
                {
                    pointer = 0;
                }
                k++;
            }
            Threaddata::instance()->wpointer = pointer;
        }
        else
        {
            Threaddata::instance()->bufferFlag = true;
            emit plotDataRequest();
            int k = 0;
            int pointer = Threaddata::instance()->wpointer;
            while (k < bufferPointNumber)
            {
                Threaddata::instance()->ys[pointer] = OneDiv * Threaddata::instance()->ybuffer2[k] + pos;
                pointer++;
                if (pointer >100)
                {
                    pointer = 0;
                }
                k++;
            }
            Threaddata::instance()->wpointer = pointer;
        }
    }


    replot();
    Threaddata::instance()->Unlock();

}

void Plot::SlotChannelOnePosChange(double pos)
{
    Threaddata::instance()->Lock();
    double OneDiv = Threaddata::instance()->ChannelOneVdiv;
    Threaddata::instance()->ChannelOnePos = pos;
    channel_one_Marker->setYValue(pos);
    int pointNumber = Threaddata::instance()->totPoint;
    int start = Threaddata::instance()->startPoint;
    int i;

    if (Threaddata::instance()->bufferFlag)
    {
        for (i=start; i<start + pointNumber; i++)
        {
            Threaddata::instance()->ys[i-start] = OneDiv * Threaddata::instance()->ybuffer2[i] + pos;
        }
    }
    else
    {
        for (i=start; i<start + pointNumber; i++)
        {
            Threaddata::instance()->ys[i-start] = OneDiv * Threaddata::instance()->ybuffer1[i] + pos;
        }
    }
    replot();


    Threaddata::instance()->Unlock();
}

void Plot::SlotChannelOneVdivChange(int index)
{
    Threaddata::instance()->Lock();

    if (index == 0)
    {
        Threaddata::instance()->ChannelOneVdiv = 1.0;
    }
    else
    {
        Threaddata::instance()->ChannelOneVdiv = 0.5;
    }
    double OneDiv = Threaddata::instance()->ChannelOneVdiv;
    int pointNumber = Threaddata::instance()->totPoint;
    double pos = Threaddata::instance()->ChannelOnePos;
    int start = Threaddata::instance()->startPoint;
    int i;

    if (Threaddata::instance()->bufferFlag)
    {
        for (i = start; i < start + pointNumber; i++)
        {
            Threaddata::instance()->ys[i-start] = OneDiv * Threaddata::instance()->ybuffer2[i] + pos;
        }
    }
    else
    {
        for (i = start; i < start + pointNumber; i++)
        {
            Threaddata::instance()->ys[i-start] = OneDiv * Threaddata::instance()->ybuffer1[i] + pos;
        }
    }
    replot();

    Threaddata::instance()->Unlock();
}

void Plot::SlotTimePosChange(int startshift)
{
    Threaddata::instance()->Lock();
    int start = (Threaddata::instance()->bufferPoint-1)/4 + startshift;
    Threaddata::instance()->startPoint = start;
    int pointNumber = Threaddata::instance()->totPoint;
    int bufNumber = Threaddata::instance()->bufferPoint;
    if (start + pointNumber <= bufNumber)
    {
        double OneDiv = Threaddata::instance()->ChannelOneVdiv;
        double pos = Threaddata::instance()->ChannelOnePos;
        int i;
        if (Threaddata::instance()->bufferFlag)
        {
            for (i = start; i < start + pointNumber; i++)
            {
                Threaddata::instance()->ys[i-start] = OneDiv * Threaddata::instance()->ybuffer2[i] + pos;
            }
        }
        else
        {
            for (i = start; i < start + pointNumber; i++)
            {
                Threaddata::instance()->ys[i-start] = OneDiv * Threaddata::instance()->ybuffer1[i] + pos;
            }
        }
        replot();
    }
    Threaddata::instance()->Unlock();
}

void Plot::SlotTimDivChange(int index)
{
    Threaddata::instance()->Lock();
        int pointnumber;
        int ratio;
        int ori_ratio = Threaddata::instance()->ori_ratio;
        switch (index)
        {
        case 0:
            pointnumber = 11;
            ratio = 1;
            break;
        case 1:
            pointnumber = 21;
            ratio = 2;
            break;
        case 2:
            pointnumber = 51;
            ratio = 5;
            break;
        case 3:
            pointnumber = 101;
            ratio = 10;
            break;
        case 4:
            pointnumber = 201;
            ratio = 20;
            break;
        }
        Threaddata::instance()->TimeDiv_change(pointnumber,ratio,ori_ratio);
        if (!Threaddata::instance()->stopFlag)
        {
            replot();
        }
Threaddata::instance()->Unlock();
        QByteArray changeTimeDivTxt = "(please change the time div data!)\n";
        //changeTimeDivTxt.prepend(char(index));
        //changeTimeDivTxt.prepend(char(0x03));
        //changeTimeDivTxt.prepend(char(0x00));
        emit TimeDivChangeMessage(&changeTimeDivTxt);
//        Threaddata::instance()->Unlock();
//    }
}
