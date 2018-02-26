#ifndef WIDGET_H
#define WIDGET_H

#include "plot.h"
//#include "samplethread.h"
#include <QWidget>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_point_data.h>
#include <QVector>
#include <QComboBox>
#include <QPushButton>
#include <QSpinBox>
#include <QGroupBox>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QHBoxLayout>

class samplethread;

class Widget : public QWidget
{
    Q_OBJECT

protected:
    void closeEvent(QCloseEvent* event);


Q_SIGNALS:
    void RunSignal();
    void changevalue(int);
    void dataRequest();
    void StopSignal();
    void TimeDivChangeMessage(QByteArray*);

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    Plot* myPlot;

    //按键设定
    QPushButton *StopBtn;
    QPushButton *RunBtn;

    //时间设定
    QComboBox *timedivCcb;
    QGroupBox *timesetting;
    QLabel *timedivLab;
    QLabel *timeposLab;
    QSpinBox *timeposSpinBox;

    //触发设定
    QGroupBox *triggersetting;
    QLabel *triggerPosLabel;
    QLabel *triggerfuncLabel;
    QComboBox *triggerfunccmb;
    QLabel *triggerslopeLabel;
    QComboBox *triggerslopecmb;
    QSpinBox *triggerPosSpinBox;
    QPushButton *setTriggerBtn;

    //通道一设定
    QGroupBox *channel_one_setting;
    QLabel *channel_one_vdiv_Label;
    QLabel *channel_one_pos_Label;
    QComboBox *channel_one_vdiv_ccb;
    QDoubleSpinBox *channel_one_pos_SpinBox;

    //通道二设定
    QGroupBox *channel_two_setting;
    QLabel *channel_two_vdiv_Label;
    QLabel *channel_two_pos_Label;
    QComboBox *channel_two_vdiv_ccb;
    QDoubleSpinBox *channel_two_pos_SpinBox;


private:
    void add_channel_ccb_item(QComboBox*);
    void add_timeCcb_item(QComboBox*);

};

#endif // WIDGET_H
