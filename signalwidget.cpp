#include "signalwidget.h"
#include <QGridLayout>
#include <QDebug>

SignalWidget::SignalWidget(QWidget *parent):
    QWidget(parent)
{
    Waveformindex = 0;
    int i;
    QPointF var;
    for (i=0;i<700;i++)
    {
        var.setX(i);
        var.setY(-150*sin(0.009*i));
        sin_table.append(var);
        if ((i!=0)&&(i<335))
        {
            var.setY(-150);
        }
        else
        {
            if (i>365)
            {
                var.setY(150);
            }
            else
            {
                var.setY(10*i-3500);
            }
        }
        square_table.append(var);
        if (i<175)
        {
            var.setY(-150.0/175*i);
        }
        else
        {
            if (i<525)
            {
                var.setY(150.0/175*i-300);
            }
            else
            {
                var.setY(600-150.0/175*i);
            }
        }
        trangle_table.append(var);
        var.setY(0);
        any_table.append(var);
    }
    map = new QPixmap(700,400);
    maplab  = new QLabel();
    maplab->setPixmap(*map);

    waveformLab = new QLabel("波形Waveform");
    amplitudeLab = new QLabel("峰峰值Vpp");
    frequencyLab = new QLabel("频率Fre");
    dcLab = new QLabel("直流偏移Voffset");
    anywaveformLab = new QLabel("任意点波形");
    waveformCmb = new QComboBox();
    waveformCmb->insertItem(0,"正弦波");
    waveformCmb->insertItem(1,"方波");
    waveformCmb->insertItem(2,"三角波");
    waveformCmb->insertItem(3,"任意波形");
    connect(waveformCmb,SIGNAL(currentIndexChanged(int)),
            this,SLOT(SlotWaveformChange(int)));
    amplitudeLET = new QLineEdit();
    frequencyLET = new QLineEdit();
    dcLET = new QLineEdit();
    anyWaveform = new QTextEdit();
    amplitudeUnit = new QLabel("V");
    frequencyUnit = new QLabel("Hz");
    dcUnit = new QLabel("V");
    anyWaveformUnit = new QLabel("V");
    sendBtn = new QPushButton("确定");
    dutyCycleLab = new QLabel("占空比");
    dutyCycleLET = new QLineEdit();
    connect(sendBtn,SIGNAL(clicked()),this,SLOT(getWaveformMessage()));
    QGridLayout *layout = new QGridLayout(this);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    layout->addWidget(waveformLab,0,0,1,1);
    layout->addWidget(waveformCmb,0,1,1,1);
    layout->addWidget(maplab,0,3,6,1);
    layout->addWidget(amplitudeLab,1,0,1,1);
    layout->addWidget(amplitudeLET,1,1,1,1);
    layout->addWidget(amplitudeUnit,1,2,1,1);

    layout->addWidget(frequencyLab,2,0,1,1);
    layout->addWidget(frequencyLET,2,1,1,1);
    layout->addWidget(frequencyUnit,2,2,1,1);

    layout->addWidget(dcLab,3,0,1,1);
    layout->addWidget(dcLET,3,1,1,1);
    layout->addWidget(dcUnit,3,2,1,1);

    layout->addWidget(dutyCycleLab, 4, 0,1,1);
    layout->addWidget(dutyCycleLET, 4, 1,1,1);

    layout->addWidget(anywaveformLab,5,0,1,1);
    layout->addWidget(anyWaveform,5,1,1,1);
    layout->addWidget(anyWaveformUnit,5,2,1,1);

    layout->addWidget(sendBtn,6,0,1,1);
    layout->setMargin(50);
    layout->setHorizontalSpacing(20);
    layout->setVerticalSpacing(20);
    //layout->setColumnStretch(1,2);
}

void SignalWidget::getWaveformMessage()
{
    int index = this->waveformCmb->currentIndex();
    QString amplitudeTxt = this->amplitudeLET->text();
    QString frequecyTxt = this->frequencyLET->text();
    QString dcTxt = this->dcLET->text();
    bool ok1;
    bool ok2;
    bool ok3;
    QByteArray patatxt = "(the signal generator parameter is XXX)\n";
    patatxt.prepend(char(0x01));
    if (index !=3)
    {
        float amp = amplitudeTxt.toFloat(&ok1);
        float fre = frequecyTxt.toFloat(&ok2);
        float dc = dcTxt.toFloat(&ok3);
        if (ok1&&ok2&&ok2)
        {
            qDebug() << index;
            qDebug() << "幅度:" << ' ' << amp;
            qDebug() << "频率:" << ' ' << fre ;
            qDebug() << "直流偏移" << ' ' << dc;
            emit sendSignalParameter(&patatxt);
        }
    }
    else
    {
        float fre = frequecyTxt.toFloat(&ok2);
        QString any = this->anyWaveform->toPlainText();
        QStringList list;
        list = any.split(QRegExp("\n"));
        float data[11] = {0.0};
        int number = list.count();
        int i;
        if ((ok2)&&(number == 11))
        {
            for (i = 0; i < 11; i++)
            {
                data[i] = list[i].toFloat();
            }
        }
        qDebug()<<index;
        qDebug()<<"频率:"<< ' ' << fre;
        for (i = 0; i < 10; i++)
        {
            qDebug()<<data[i]<<' ';
        }
        emit sendSignalParameter(&patatxt);

    }
}

void SignalWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(map);
    painter.setPen(Qt::yellow);
    painter.translate(0,200);
    map->fill(Qt::black);
    switch(Waveformindex)
    {
    case 0:
        painter.drawLines(sin_table);
        break;
    case 1:
        painter.drawLines(square_table);
        break;
    case 2:
        painter.drawLines(trangle_table);
        break;
    case 3:
        painter.drawLines(any_table);
        break;
    }

    maplab->setPixmap(*map);
}

void SignalWidget::SlotWaveformChange(int index)
{
    Waveformindex = index;
    update();
}
