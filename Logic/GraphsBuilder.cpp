#include "GraphsBuilder.h"
#include "OrderBook.h"


#include <QtGlobal>
#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>

using namespace QtCharts;

MarketDepthGraph* GraphsBuilder::buildMarketDepthGraph(OrderBook* orderBook) {

    //объ€вл€ем серии дл€ хранени€ координат точек линий
    QLineSeries* bidsUpLineSeries = new QLineSeries();
    QLineSeries* bidsDownLineSeries = new QLineSeries();
    QLineSeries* asksUpLineSeries = new QLineSeries();
    QLineSeries* asksDownLineSeries = new QLineSeries();


    std::map<long long, int>* bids = &orderBook->bidsAmountForPrice;
    std::map<long long, int>* asks = &orderBook->asksAmountForPrice;

    //получаем координаты из мапов
    auto iter = asks->begin();
    int prevY = iter->second;
    asksUpLineSeries->append(qreal(iter->first), qreal(iter->second));
    iter++;
    for (; iter != asks->end(); iter++) {
        asksUpLineSeries->append(qreal(iter->first), qreal(prevY));
        asksUpLineSeries->append(qreal(iter->first), qreal((iter->second) + prevY));
        prevY += iter->second;
    }

    iter = bids->end(); iter--;
    prevY = iter->second;
    bidsUpLineSeries->append(qreal(iter->first), qreal(iter->second));
    do {
        iter--;
        bidsUpLineSeries->append(qreal(iter->first), qreal(prevY));
        bidsUpLineSeries->append(qreal(iter->first), qreal((iter->second) + prevY));
        prevY += iter->second;
    } while (iter != bids->begin());

    iter = bids->end(); iter--;
    *bidsDownLineSeries << QPointF(qreal(iter->first), 0) << QPointF(qreal(bids->begin()->first), 0);
    iter = asks->end(); iter--;
    *asksDownLineSeries << QPointF(qreal(asks->begin()->first), 0) << QPointF(qreal(iter->first), 0);



    QAreaSeries* bidsSeries = new QAreaSeries(bidsUpLineSeries, bidsDownLineSeries);
    bidsSeries->setName("Bids");
    QPen pen(0x059605);
    pen.setWidth(1);
    bidsSeries->setPen(pen);

    QAreaSeries* asksSeries = new QAreaSeries(asksUpLineSeries, asksDownLineSeries);
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

    //создаем макет графика

    auto marketDepthChart = new MarketDepthGraph();
    marketDepthChart->addSeries(bidsSeries);
    marketDepthChart->addSeries(asksSeries);
    marketDepthChart->setTitle("Market Depth");
    marketDepthChart->createDefaultAxes();

    //будуща€ фича дл€ расширени€ полей у оси Y

    //marketDepthChart->axes(Qt::Horizontal).first()->setRange(0, 20);
    //marketDepthChart->axes(Qt::Vertical).first()->setRange(0, 10);

    return marketDepthChart;
/*
    QChartView* MarketDepthView = new QChartView(marketDepthChart);
    MarketDepthView->setRenderHint(QPainter::Antialiasing);
*/

}