#include "GraphsBuilder.h"
#include "OrderBook.h"

#include <QtGlobal>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>

using namespace QtCharts;

void GraphsBuilder::buildMarketDepthGraph(const OrderBook* orderBook) {

    std::map<long long, int> bids = orderBook->bidsAmountForPrice;
    std::map<long long, int> asks = orderBook->asksAmountForPrice;

        auto iter = asks.begin();
    int prevY = iter->second;
    asksUpLineSeries->append(qreal(iter->first), qreal(iter->second));
    iter++;
    for (/*    */; iter != asks.end(); iter++) {
        asksUpLineSeries->append(qreal(iter->first), qreal(prevY));
        asksUpLineSeries->append(qreal(iter->first), qreal((iter->second) + prevY));
        prevY += iter->second;
    }

    iter = bids.end(); iter--;
    auto tempBeginIter = bids.begin();
    tempBeginIter--;
    prevY = iter->second;
    bidsUpLineSeries->append(qreal(iter->first), qreal(iter->second));
    iter--;
    for (/*    */; iter != tempBeginIter; iter--) {
        bidsUpLineSeries->append(qreal(iter->first), qreal(prevY));
        bidsUpLineSeries->append(qreal(iter->first), qreal((iter->second) + prevY));
        prevY += iter->second;
    }
    iter = bids.end(); iter--;
    *bidsDownLineSeries << QPointF(qreal(iter->first), 0) << QPointF(qreal(bids.begin()->first), 0);
    iter = asks.end(); iter--;
    *asksDownLineSeries << QPointF(qreal(asks.begin()->first), 0) << QPointF(qreal(iter->first), 0);



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
    gradient.setColorAt(0.0, 0x3cc63c);
    gradient.setColorAt(1.0, 0x26f626);
    gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    bidsSeries->setBrush(gradient);
    gradient.setColorAt(0.0, 0xcc0000);
    gradient.setColorAt(1.0, 0xe60000);
    asksSeries->setBrush(gradient);

    QChart* chart = new QChart();
    chart->addSeries(bidsSeries);
    chart->addSeries(asksSeries);
    chart->setTitle("Market Depth");
    chart->createDefaultAxes();
    //chart->axes(Qt::Horizontal).first()->setRange(0, 20);
    //chart->axes(Qt::Vertical).first()->setRange(0, 10);

    QChartView* MarketDepthView = new QChartView(chart);
    MarketDepthView->setRenderHint(QPainter::Antialiasing);


}