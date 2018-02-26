#ifndef PLOT_H
#define PLOT_H

#include <qwt_plot.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>

class Plot:public QwtPlot
{
    Q_OBJECT

public:
    Plot(QWidget * parent = NULL);
    virtual ~Plot();
    virtual void replot();
    void start();

private:
    QwtPlotMarker *d_origin;
    QwtPlotCurve *d_curve;
    QwtPlotMarker *channel_one_Marker;
    int d_timerID;
    void updateCurve();

protected:
    virtual void timerEvent( QTimerEvent * );

signals:
    void plotDataRequest();

Q_SIGNALS:
    void TimeDivChangeMessage(QByteArray*);

public slots:
    void SlotChannelOnePosChange(double);
    void SlotChannelOneVdivChange(int);
    void SlotTimePosChange(int);
    void SlotTimDivChange(int);
};

#endif // PLOT_H
