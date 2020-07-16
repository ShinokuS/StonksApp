﻿#include "GraphsBuilder.h"

#include <QtGlobal>
#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>
#include <QDateTime>

using namespace QtCharts;

bool isFirstDeal;

MarketDepthGraph* GraphsBuilder::buildMarketDepthGraph(OrderBookTableModel* orderBook) {

    // Объявляем серии для хранения точек для линий графика.
    QLineSeries* bidsUpLineSeries = new QLineSeries();
    QLineSeries* bidsDownLineSeries = new QLineSeries();
    QLineSeries* asksUpLineSeries = new QLineSeries();
    QLineSeries* asksDownLineSeries = new QLineSeries();

    // Обрабатываем данные с модели
    auto iter = orderBook->rows.end();
    do iter--; while (! (*iter)->isAsk);
    auto centralIter = iter;
    qreal prevY = (*iter)->quantity;
    asksUpLineSeries->append(prevY, (*iter)->price);
    do {
        iter--;
        asksUpLineSeries->append(prevY, (*iter)->price);
        asksUpLineSeries->append(prevY + (*iter)->quantity, (*iter)->price);
        prevY += (*iter)->quantity;
    } while (iter!=orderBook->rows.begin());
    *asksDownLineSeries << QPointF(0, (*centralIter)->price) << QPointF(0, (*iter)->price);

    iter = centralIter++;
    prevY = (*iter)->quantity;
    iter++;
    for (; iter != orderBook->rows.end(); iter++) {
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

PriceGraph* GraphsBuilder::buildPriceGraph(Deals* dealsModel)
{
    auto priceGraph = new PriceGraph(getTimeForLinePriceGraph(dealsModel),
                                    getPriceForLinePriceGraph(dealsModel));
    
    if (GraphsBuilder::getTimeForLinePriceGraph(dealsModel).empty()) {
        isFirstDeal = true;
    }
    else {
        int firstDayTime = (int(GraphsBuilder::getTimeForLinePriceGraph(dealsModel).first()) / 86400) * 86400 - 10800;
        int lastDayTime = firstDayTime + 86400;
        priceGraph->xAxis->setRange(firstDayTime, lastDayTime);
        isFirstDeal = false;
    }

    return priceGraph;
}

void GraphsBuilder::update(PriceGraph* priceGraph, Deals* dealsModel)
{
    priceGraph->graph()->clearData();
    if (!GraphsBuilder::getTimeForLinePriceGraph(dealsModel).empty()) {
        if (isFirstDeal) {
            int firstDayTime = (int(GraphsBuilder::getTimeForLinePriceGraph(dealsModel).first()) / 86400) * 86400 - 10800;
            int lastDayTime = firstDayTime + 86400;
            priceGraph->xAxis->setRange(firstDayTime, lastDayTime);
            isFirstDeal = false;
        }
        priceGraph->graph()->clearData();
        priceGraph->graph()->setData(GraphsBuilder::getTimeForLinePriceGraph(dealsModel), GraphsBuilder::getPriceForLinePriceGraph(dealsModel));
        priceGraph->yAxis->setRange(dealsModel->dealsForLineGraph.last()->price, dealsModel->maxPrice, Qt::AlignBottom);
    }
    priceGraph->replot();
}

QVector<double> GraphsBuilder::getTimeForLinePriceGraph(Deals* deals)
{
    QVector <double> time;
    for (auto iter = deals->dealsForLineGraph.begin(); iter != deals->dealsForLineGraph.end(); iter++) {
        time.push_back((*iter)->time);
    }
    return time;
}
QVector<double> GraphsBuilder::getPriceForLinePriceGraph(Deals* deals)
{
    QVector <double> price;
    for (auto iter = deals->dealsForLineGraph.begin(); iter != deals->dealsForLineGraph.end(); iter++) {
        price.push_back((*iter)->price);
    }
    return price;
}
