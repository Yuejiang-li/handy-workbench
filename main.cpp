#include "widget.h"
#include "maintabwidget.h"
#include "datarcvthread.h"
#include <QApplication>
#include <QTabWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Widget w;
    DataRcvThread myDataRcvThread;
    MainTabWidget maintabWidget;
    //w.show();
    maintabWidget.connect(&maintabWidget,SIGNAL(RunSignal()),&myDataRcvThread,SLOT(startRunning()),Qt::QueuedConnection);
    maintabWidget.connect(&maintabWidget,SIGNAL(dataRequest()),&myDataRcvThread,SLOT(sendRequestMessage()),Qt::QueuedConnection);
    maintabWidget.connect(&maintabWidget,SIGNAL(sendSignalParameter(QByteArray*)),&myDataRcvThread,SLOT(SlotSendSignalParameter(QByteArray*)));
    maintabWidget.connect(&maintabWidget,SIGNAL(StopSignal()),&myDataRcvThread,SLOT(SlotStopSignal()));
    maintabWidget.connect(&maintabWidget,SIGNAL(TimeDivChangeMessage(QByteArray*)),&myDataRcvThread,SLOT(SlotTimeDivChangeMessage(QByteArray*)));
    maintabWidget.show();
    myDataRcvThread.start();
    bool ok = a.exec();

    return ok;
}
