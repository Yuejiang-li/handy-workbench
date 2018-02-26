#include "widget.h"
#include "plot.h"
#include <qwt_plot_grid.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_curve.h>
#include <QVector>
#include <QtMath>
#include <QCloseEvent>
#include <QGridLayout>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{

    //时间设定
    timesetting = new QGroupBox(QObject::tr("水平轴"));
    timedivLab = new QLabel(timesetting);
    timedivLab->setText(QObject::tr("水平时基"));
    timeposLab = new QLabel(timesetting);
    timeposLab->setText(QObject::tr("水平位置"));
    timedivCcb = new QComboBox(timesetting);
    timeposSpinBox = new QSpinBox(timesetting);
    add_timeCcb_item(timedivCcb);
    QGridLayout *timesettingLayout = new QGridLayout(timesetting);
    timesettingLayout->addWidget(timedivLab,0,0);
    timesettingLayout->addWidget(timedivCcb,1,0);
    timesettingLayout->addWidget(timeposLab,0,1);
    timesettingLayout->addWidget(timeposSpinBox,1,1);
    timesettingLayout->setMargin(30);
    timesettingLayout->setVerticalSpacing(10);
    timesettingLayout->setHorizontalSpacing(40);
    timesettingLayout->setAlignment(timesetting,Qt::AlignHCenter|Qt::AlignVCenter);

    //触发控件的初始化
    triggersetting = new QGroupBox(QObject::tr("触发设置"));
    triggerPosLabel = new QLabel(triggersetting);
    triggerPosLabel->setText(QObject::tr("触发位置"));
    triggerfuncLabel = new QLabel(triggersetting);
    triggerfuncLabel->setText(QObject::tr("触发方式"));
    triggerslopeLabel = new QLabel(triggersetting);
    triggerslopeLabel->setText(QObject::tr("触发沿选择"));
    triggerfunccmb =new QComboBox(triggersetting);
    triggerfunccmb->insertItem(0,"自动");
    triggerfunccmb->insertItem(1,"单次");
    triggerslopecmb = new QComboBox(triggersetting);
    triggerslopecmb->insertItem(0,"上升沿");
    triggerslopecmb->insertItem(1,"下降沿");
    triggerslopecmb->insertItem(2,"双沿");
    triggerPosSpinBox = new QSpinBox(triggersetting);
    setTriggerBtn = new QPushButton(QObject::tr("触发"),triggersetting);
    QGridLayout *triggerLayout = new QGridLayout(triggersetting);
    triggerLayout->addWidget(triggerPosLabel,0,0);
    triggerLayout->addWidget(triggerPosSpinBox,0,1);
    triggerLayout->addWidget(setTriggerBtn,3,1);
    triggerLayout->addWidget(triggerfuncLabel,1,0);
    triggerLayout->addWidget(triggerfunccmb,1,1);
    triggerLayout->addWidget(triggerslopeLabel,2,0);
    triggerLayout->addWidget(triggerslopecmb,2,1);
    triggerLayout->setMargin(30);
    triggerLayout->setVerticalSpacing(10);
    triggerLayout->setHorizontalSpacing(40);
    triggerLayout->setAlignment(triggersetting,Qt::AlignHCenter|Qt::AlignVCenter);

    //通道一设定
    channel_one_setting = new QGroupBox(QObject::tr("通道一"));
    channel_one_vdiv_Label = new QLabel(channel_one_setting);
    channel_one_vdiv_Label->setText(QObject::tr("垂直分辨率"));
    channel_one_pos_Label = new QLabel(channel_one_setting);
    channel_one_pos_Label->setText(QObject::tr("垂直位置"));
    channel_one_vdiv_ccb = new QComboBox(channel_one_setting);
    add_channel_ccb_item(channel_one_vdiv_ccb);


    channel_one_pos_SpinBox = new QDoubleSpinBox(channel_one_setting);
    channel_one_pos_SpinBox->setRange(-5.0, 5.0);
    QGridLayout *channel_one_Layout = new QGridLayout(channel_one_setting);
    channel_one_Layout->addWidget(channel_one_vdiv_Label,0,0);
    channel_one_Layout->addWidget(channel_one_pos_Label,0,2);
    channel_one_Layout->addWidget(channel_one_vdiv_ccb,1,0);
    channel_one_Layout->addWidget(channel_one_pos_SpinBox,1,2);
    channel_one_Layout->setMargin(30);
    channel_one_Layout->setVerticalSpacing(10);
    channel_one_Layout->setHorizontalSpacing(40);
    channel_one_Layout->setAlignment(channel_one_setting,Qt::AlignHCenter|Qt::AlignVCenter);

    //通道二设定
    channel_two_setting = new QGroupBox(QObject::tr("通道二"));
    channel_two_vdiv_Label = new QLabel(channel_two_setting);
    channel_two_vdiv_Label->setText(QObject::tr("垂直分辨率"));
    channel_two_pos_Label = new QLabel(channel_two_setting);
    channel_two_pos_Label->setText(QObject::tr("垂直位置"));
    channel_two_vdiv_ccb = new QComboBox(channel_two_setting);
    add_channel_ccb_item(channel_two_vdiv_ccb);
    channel_two_pos_SpinBox = new QDoubleSpinBox(channel_two_setting);
    channel_two_pos_SpinBox->setRange(-5.0, 5.0);
    QGridLayout *channel_two_Layout = new QGridLayout(channel_two_setting);
    channel_two_Layout->addWidget(channel_two_vdiv_Label,0,0);
    channel_two_Layout->addWidget(channel_two_pos_Label,0,2);
    channel_two_Layout->addWidget(channel_two_vdiv_ccb,1,0);
    channel_two_Layout->addWidget(channel_two_pos_SpinBox,1,2);
    channel_two_Layout->setMargin(30);
    channel_two_Layout->setVerticalSpacing(10);
    channel_two_Layout->setHorizontalSpacing(40);
    channel_two_Layout->setAlignment(channel_two_setting,Qt::AlignHCenter|Qt::AlignVCenter);

    StopBtn = new QPushButton("结束");
    RunBtn = new QPushButton("运行");
    connect(RunBtn,SIGNAL(clicked()),this,SIGNAL(RunSignal()));
    connect(StopBtn,SIGNAL(clicked()),this,SIGNAL(StopSignal()));
    QHBoxLayout *BtnLayout = new QHBoxLayout();
    BtnLayout->addWidget(RunBtn);
    BtnLayout->addWidget(StopBtn);
    BtnLayout->setMargin(30);
    BtnLayout->setSpacing(40);

    QGridLayout *RightLayout = new QGridLayout();
    RightLayout->addWidget(channel_one_setting,0,0,1,1);
    RightLayout->addWidget(channel_two_setting,0,1,1,1);
    RightLayout->addWidget(triggersetting,1,0,1,1);
    RightLayout->addWidget(timesetting,1,1,1,1);
    RightLayout->addLayout(BtnLayout,2,0,1,2);
    RightLayout->setMargin(20);
    RightLayout->setVerticalSpacing(30);
    RightLayout->setHorizontalSpacing(20);

    myPlot = new Plot();
    connect(myPlot,SIGNAL(plotDataRequest()),this,SIGNAL(dataRequest()));
    connect(channel_one_pos_SpinBox, SIGNAL(valueChanged(double)),
            myPlot, SLOT(SlotChannelOnePosChange(double)));
    connect(channel_one_vdiv_ccb, SIGNAL(currentIndexChanged(int)),
            myPlot, SLOT(SlotChannelOneVdivChange(int)));
    connect(timeposSpinBox, SIGNAL(valueChanged(int)),
            myPlot, SLOT(SlotTimePosChange(int)));
    connect(timedivCcb,SIGNAL(currentIndexChanged(int)),
            myPlot,SLOT(SlotTimDivChange(int)));
    connect(myPlot, SIGNAL(TimeDivChangeMessage(QByteArray*)),
            this, SIGNAL(TimeDivChangeMessage(QByteArray*)));
    myPlot->start();

    QHBoxLayout *MainLayout = new QHBoxLayout(this);
    MainLayout->addWidget(myPlot);
    MainLayout->addLayout(RightLayout);
    MainLayout->setSpacing(20);
}

Widget::~Widget()
{

}

void Widget::closeEvent(QCloseEvent* event)
{
}


void Widget::add_channel_ccb_item(QComboBox *ccb1)
{
    ccb1->insertItem(0,"1 mV/div");
    ccb1->insertItem(1,"2 mV/div");
    ccb1->insertItem(2,"5 mv/div");
    ccb1->insertItem(3,"10 mv/div");
    ccb1->insertItem(4,"20 mv/div");
    ccb1->insertItem(5,"50 mv/div");
    ccb1->insertItem(6,"100 mv/div");
    ccb1->insertItem(7,"200 mv/div");
    ccb1->insertItem(8,"500 mv/div");
    ccb1->insertItem(9,"1 V/div");
    ccb1->insertItem(10,"2 V/div");
    ccb1->insertItem(11,"5 V/div");
    ccb1->insertItem(12,"10 V/div");
}

void Widget::add_timeCcb_item(QComboBox *Ccb)
{
    Ccb->insertItem(0,"1 us/div");
    Ccb->insertItem(1,"2 us/div");
    Ccb->insertItem(2,"5 us/div");
    Ccb->insertItem(3,"10 us/div");
    Ccb->insertItem(4,"20 us/div");
    Ccb->insertItem(5,"50 us/div");
    Ccb->insertItem(6,"100 us/div");
    Ccb->insertItem(7,"200 us/div");
    Ccb->insertItem(8,"500 us/div");
    Ccb->insertItem(9,"1 ms/div");
    Ccb->insertItem(10,"2 ms/div");
    Ccb->insertItem(11,"5 ms/div");
    Ccb->insertItem(12,"10 ms/div");
    Ccb->insertItem(13,"20 ms/div");
    Ccb->insertItem(14,"50 ms/div");
    Ccb->insertItem(15,"100 ms/div");
    Ccb->insertItem(16,"200 ms/div");
    Ccb->insertItem(17,"500 ms/div");
    Ccb->insertItem(18,"1 s/div");
}
