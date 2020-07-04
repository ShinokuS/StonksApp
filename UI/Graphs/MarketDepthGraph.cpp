#include "MarketDepthGraph.h"

#include <QtCharts/QAreaSeries>

// Конструктор получает на вход готовые линии графика
// и отстраивает визуальный стиль их отображения

MarketDepthGraph::MarketDepthGraph(QAreaSeries* bidsSeries, QAreaSeries* asksSeries)
{
    bidsSeries->setName("Bids");
    QPen pen(0x059605);
    pen.setWidth(1);
    bidsSeries->setPen(pen);

    asksSeries->setName("Asks");
    pen.setColor(0x940000);
    pen.setWidth(1);
    asksSeries->setPen(pen);

    QLinearGradient gradient(QPointF(0, 0), QPointF(0, 1));
    gradient.setColorAt(0.0, 0x26f626);
    gradient.setColorAt(1.0, 0x1e671e);
    gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    bidsSeries->setBrush(gradient);
    gradient.setColorAt(0.0, 0xe60000);
    gradient.setColorAt(1.0, 0x5c0006);
    asksSeries->setBrush(gradient);

    this->addSeries(bidsSeries);
    this->addSeries(asksSeries);
    this->setTitle("Market Depth");
    this->createDefaultAxes();
    

    //будущая фича для расширения полей у оси Y

    //this->axes(Qt::Horizontal).first()->setRange(0, 20);
    //this->axes(Qt::Vertical).first()->setRange(0, 10);
}