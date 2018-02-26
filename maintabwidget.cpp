#include "maintabwidget.h"


MainTabWidget::MainTabWidget(QWidget * parent):
    QTabWidget(parent)
{
    w = new Widget();
    signalwidget = new SignalWidget();
    this->addTab(w,"示波器");
    this->addTab(signalwidget,"信号发生器");
    connect(w,SIGNAL(RunSignal()),this,SIGNAL(RunSignal()));
    connect(w,SIGNAL(dataRequest()),this,SIGNAL(dataRequest()));
    connect(w,SIGNAL(StopSignal()),this,SIGNAL(StopSignal()));
    connect(signalwidget,SIGNAL(sendSignalParameter(QByteArray*)),this,SIGNAL(sendSignalParameter(QByteArray*)));
    connect(w,SIGNAL(TimeDivChangeMessage(QByteArray*)),
            this,SIGNAL(TimeDivChangeMessage(QByteArray*)));
}
