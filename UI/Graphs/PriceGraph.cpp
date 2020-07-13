#include "PriceGraph.h"

LinePriceGraph::LinePriceGraph(QLineSeries* lineSeries)
{
    //QDateTime min_time;
    //QDateTime max_time;

    //min_time.setTime(QTime(0, 0));
    //max_time.setTime(QTime(23, 59));

    this->legend()->hide();
    this->addSeries(lineSeries);
    QDateTimeAxis* axisX = new QDateTimeAxis;
    axisX->setTickCount(12);
    axisX->setFormat("hh:mm");
    //axisX->setRange(min_time, max_time);
    axisX->setTitleText("Time");
    this->addAxis(axisX, Qt::AlignBottom);
    lineSeries->attachAxis(axisX);

    QValueAxis* axisY = new QValueAxis;
    //axisY->setLabelFormat("%i");
    axisY->setTitleText("Price");
    this->addAxis(axisY, Qt::AlignLeft);
    lineSeries->attachAxis(axisY);

    this->setTitle("Market price");
}
