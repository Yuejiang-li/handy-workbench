#ifndef MAINTABWIDGET_H
#define MAINTABWIDGET_H

#include <QTabWidget>
#include "widget.h"
#include "signalwidget.h"

class MainTabWidget:public QTabWidget
{
    Q_OBJECT

public:
    MainTabWidget(QWidget *parent = NULL);
    Widget* w;
    SignalWidget* signalwidget;

Q_SIGNALS:
    void RunSignal();
    void StopSignal();
    void dataRequest();
    void sendSignalParameter(QByteArray*);
    void TimeDivChangeMessage(QByteArray*);

};

#endif // MAINTABWIDGET_H
