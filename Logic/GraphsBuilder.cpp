#include "GraphsBuilder.h"

#include <QtGlobal>
#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>
#include <QDateTime>

using namespace QtCharts;

MarketDepthGraph* GraphsBuilder::buildMarketDepthGraph(OrderBook* orders) 
{

    this->orderBook = orders;

    auto marketDepthGraph = new MarketDepthGraph(getPriceAskForMarketDepthGraph(),
        getQuantityAskForMarketDepthGraph(), getPriceBidForMarketDepthGraph(),
        getQuantityBidForMarketDepthGraph());

    if (getPriceAskForMarketDepthGraph().empty() || 
        getPriceBidForMarketDepthGraph().empty()) {
        isFirstOrder = true;
    }
    else {
        marketDepthGraph->rescaleAxes();
        isFirstOrder = false;
    }

    return marketDepthGraph;
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
void GraphsBuilder::updateMarketDepthGraph(MarketDepthGraph* marketDepthGraph)
{
    marketDepthGraph->graph(0)->clearData();
    marketDepthGraph->graph(1)->clearData();
    if (!getPriceAskForMarketDepthGraph().empty() &&
        !getPriceBidForMarketDepthGraph().empty()) {
            
        marketDepthGraph->graph(0)->setData(getPriceAskForMarketDepthGraph(),
            getQuantityAskForMarketDepthGraph());
        marketDepthGraph->graph(1)->setData(getPriceBidForMarketDepthGraph(),
                getQuantityBidForMarketDepthGraph());
    }
    marketDepthGraph->rescaleAxes();
    marketDepthGraph->replot();
}

void GraphsBuilder::updatePriceGraph(PriceGraph* priceGraph, BotLogic* bot)
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
            priceGraph->graph(1)->setData(bot->timeBuy, bot->priceBuy);
            priceGraph->graph(2)->setData(bot->timeSell, bot->priceSell);
            priceGraph->yAxis->setRange(dealsModel->dealsForPriceGraph.last()->price,
                2*std::max(dealsModel->dealsForPriceGraph.last()->price-minPrice,
                    maxPrice - dealsModel->dealsForPriceGraph.last()->price)+1, Qt::AlignBottom);
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
        if ((*iter)->price > maxPrice) {
            maxPrice = (*iter)->price;
        }
        if ((*iter)->price < minPrice || minPrice == 0) {
            minPrice = (*iter)->price;
        }
    }
    return price;
}

QVector<double> GraphsBuilder::getPriceAskForMarketDepthGraph()
{
    QVector <double> price;
    auto iter = orderBook->orders.end();
    do iter--; while (!(*iter)->isAsk);
    price.append((*iter)->price);
    do {
        iter--;
        price.append((*iter)->price);
        price.append((*iter)->price);
    } while (iter != orderBook->orders.begin());
    return price;
}

QVector<double> GraphsBuilder::getPriceBidForMarketDepthGraph()
{
    QVector <double> price;
    auto iter = orderBook->orders.end();
    do iter--; while (!(*iter)->isAsk);
    iter++; iter++;
    for (; iter != orderBook->orders.end(); iter++) {
        price.append((*iter)->price);
        price.append((*iter)->price);
    }
    return price;
}

QVector<double> GraphsBuilder::getQuantityAskForMarketDepthGraph()
{
    QVector <double> quantity;
    auto iter = orderBook->orders.end();
    do iter--; while (!(*iter)->isAsk);
    double prevY = (*iter)->quantity;
    quantity.append(prevY);
    do {
        iter--;
        quantity.append(prevY);
        quantity.append(prevY+(*iter)->quantity);
        prevY += (*iter)->quantity;
    } while (iter != orderBook->orders.begin());
    return quantity;
}

QVector<double> GraphsBuilder::getQuantityBidForMarketDepthGraph() {
    QVector <double> quantity;
    auto iter = orderBook->orders.end();
    do iter--; while (!(*iter)->isAsk);
    iter++; 
    double prevY = (*iter)->quantity;
    iter++;
    for (; iter != orderBook->orders.end(); iter++) {
        quantity.append(prevY);
        quantity.append(prevY + (*iter)->quantity);
        prevY += (*iter)->quantity;
    }
    return quantity;
}