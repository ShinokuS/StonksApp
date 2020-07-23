#include "GraphsBuilder.h"

#include <QtGlobal>
#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>
#include <QDateTime>

using namespace QtCharts;

MarketDepthGraph* GraphsBuilder::buildMarketDepthGraph(OrderBook* orderBook) {

    // Объявляем серии для хранения точек для линий графика.
    QLineSeries* bidsUpLineSeries = new QLineSeries();
    QLineSeries* bidsDownLineSeries = new QLineSeries();
    QLineSeries* asksUpLineSeries = new QLineSeries();
    QLineSeries* asksDownLineSeries = new QLineSeries();

    // Обрабатываем данные с модели
    auto iter = orderBook->orders.end();
    do iter--; while (! (*iter)->isAsk);
    auto centralIter = iter;
    qreal prevY = (*iter)->quantity;
    asksUpLineSeries->append(prevY, (*iter)->price);
    do {
        iter--;
        asksUpLineSeries->append(prevY, (*iter)->price);
        asksUpLineSeries->append(prevY + (*iter)->quantity, (*iter)->price);
        prevY += (*iter)->quantity;
    } while (iter!=orderBook->orders.begin());
    *asksDownLineSeries << QPointF(0, (*centralIter)->price) << QPointF(0, (*iter)->price);

    iter = centralIter++;
    prevY = (*iter)->quantity;
    iter++;
    for (; iter != orderBook->orders.end(); iter++) {
        bidsUpLineSeries->append(prevY, (*iter)->price);
        bidsUpLineSeries->append(prevY + (*iter)->quantity, (*iter)->price);
        prevY += (*iter)->quantity;
    }
    iter--;
    *bidsDownLineSeries << QPointF(0, (*centralIter)->price) << QPointF(0, (*iter)->price);

    // Получаем полную форму каждой половины графика по кривой сверху и дну снизу.
    QAreaSeries* bidsSeries = new QAreaSeries(bidsUpLineSeries, bidsDownLineSeries);
    QAreaSeries* asksSeries = new QAreaSeries(asksUpLineSeries, asksDownLineSeries);

    // В график запихиваем все эти полученные линии,
    // а он в конструкторе разбирается со стилем их отображения.
    // На выходе из метода будет полностью готовый график.
    return new MarketDepthGraph(bidsSeries, asksSeries);
}

PriceGraph* GraphsBuilder::buildPriceGraph(Deals* deals)
{
    // сохраняем на будущее
    this->dealsModel = deals;

    auto priceGraph = new PriceGraph(getTimeForPriceGraph(),
                                    getPriceForPriceGraph());
    
    if (getTimeForPriceGraph().empty()) {
        isFirstDeal = true;
    }
    else {
        priceGraph->xAxis->setRange(dealsModel->dealsForPriceGraph.last()->time,
            dealsModel->dealsForPriceGraph.last()->time + Time::THREE_HOURS);
        priceGraph->yAxis->setRange(dealsModel->dealsForPriceGraph.last()->price,
                                    dealsModel->dealsForPriceGraph.last()->price, Qt::AlignBottom);
        isFirstDeal = false;
    }

    return priceGraph;
}

void GraphsBuilder::update(PriceGraph* priceGraph)
{
    priceGraph->graph()->clearData();
    if (! getTimeForPriceGraph().empty()) {
        if (isFirstDeal) {
            priceGraph->xAxis->setRange(dealsModel->dealsForPriceGraph.first()->time,
                dealsModel->dealsForPriceGraph.last()->time + Time::THREE_HOURS);
            priceGraph->yAxis->setRange(dealsModel->dealsForPriceGraph.last()->price,
                dealsModel->dealsForPriceGraph.last()->price, Qt::AlignBottom);
            isFirstDeal = false;
        }
        else {
            priceGraph->graph(0)->setData(getTimeForPriceGraph(),
                getPriceForPriceGraph());
            priceGraph->yAxis->setRange(dealsModel->dealsForPriceGraph.last()->price,
                dealsModel->dealsForPriceGraph.last()->price, Qt::AlignBottom);
        }
    }
    priceGraph->replot();
}

QVector<double> GraphsBuilder::getTimeForPriceGraph()
{
    QVector <double> time;
    for (auto iter = dealsModel->dealsForPriceGraph.begin();
            iter != dealsModel->dealsForPriceGraph.end(); iter++) {
        time.push_back((*iter)->time);
    }
    return time;
}
QVector<double> GraphsBuilder::getPriceForPriceGraph()
{
    QVector <double> price;
    for (auto iter = dealsModel->dealsForPriceGraph.begin();
            iter != dealsModel->dealsForPriceGraph.end(); iter++) {
        price.push_back((*iter)->price);
    }
    return price;
}
