#ifndef SIGNALWIDGET_H
#define SIGNALWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QPixmap>
#include <QPainter>
#include <QVector>
#include <QPointF>

class SignalWidget:public QWidget
{
    Q_OBJECT

public:
    SignalWidget(QWidget *parent = 0);

private:
    QComboBox *waveformCmb;
    QLineEdit *amplitudeLET;
    QLineEdit *frequencyLET;
    QLineEdit *dcLET;
    QLineEdit *dutyCycleLET;
    QTextEdit *anyWaveform;
    QLabel *waveformLab;
    QLabel *amplitudeLab;
    QLabel *frequencyLab;
    QLabel *dcLab;
    QLabel *anywaveformLab;
    QLabel *amplitudeUnit;
    QLabel *frequencyUnit;
    QLabel *dcUnit;
    QLabel *anyWaveformUnit;
    QLabel *dutyCycleLab;
    QPushButton *sendBtn;
    QPixmap *map;
    QLabel *maplab;
    QVector<QPointF> sin_table;
    QVector<QPointF> square_table;
    QVector<QPointF> trangle_table;
    QVector<QPointF> any_table;
    int Waveformindex;

public slots:
    void getWaveformMessage();
    void SlotWaveformChange(int);

protected:
    void paintEvent(QPaintEvent*);

Q_SIGNALS:
    void sendSignalParameter(QByteArray*);
};

#endif // SIGNALWIDGET_H
