#include "GraphsBuilder.h"
#include "OrderBook.h"
#include "..\Utility\PriceAsQReal.h"

#include <QtGlobal>
#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>

using namespace QtCharts;

// iter->first само по себе длинновато и не понятно,
// а его ещё надо конвертнуть в qreal и разделить на 100
#define price priceAsQReal(iter->first)

// тут аналогично, но без деления
#define nOrders qreal(iter->second)


MarketDepthGraph* GraphsBuilder::buildMarketDepthGraph(OrderBook* orderBook) {

    // Краткие псевдонимы для мапов.
    std::map<long long, int>* bids = &orderBook->bidsAmountForPrice;
    std::map<long long, int>* asks = &orderBook->asksAmountForPrice;

    // Объявляем серии для хранения точек для линий графика.
    QLineSeries* bidsUpLineSeries = new QLineSeries();
    QLineSeries* bidsDownLineSeries = new QLineSeries();
    QLineSeries* asksUpLineSeries = new QLineSeries();
    QLineSeries* asksDownLineSeries = new QLineSeries();

    // Получаем координаты из одного мапа.
    qreal prevY = 0;
    for (auto iter = asks->begin(); iter != asks->end(); iter++) {
        asksUpLineSeries->append(prevY, price);
        asksUpLineSeries->append(prevY + nOrders, price);
        prevY += iter->second;
    }

    // И из другого.
    auto iter = bids->end();
    iter--;
    prevY = iter->second;
    bidsUpLineSeries->append(nOrders, price);
    do {
        iter--;
        bidsUpLineSeries->append(prevY, price);
        bidsUpLineSeries->append(prevY + nOrders, price);
        prevY += iter->second;
    } while (iter != bids->begin());

    // Рисуем дно каждой половины графика.
    iter = bids->end();
    iter--;
    qreal firstBidPrice = priceAsQReal(bids->begin()->first);
    *bidsDownLineSeries << QPointF(0,price) << QPointF(0, firstBidPrice);
    
    iter = asks->end();
    iter--;
    qreal firstAskPrice = priceAsQReal(asks->begin()->first);
    *asksDownLineSeries << QPointF(0, firstBidPrice) << QPointF(0, price);

    // Получаем полную форму каждой половины графика по кривой сверху и дну снизу.
    QAreaSeries* bidsSeries = new QAreaSeries(bidsUpLineSeries, bidsDownLineSeries);
    QAreaSeries* asksSeries = new QAreaSeries(asksUpLineSeries, asksDownLineSeries);

    // В график запихиваем все эти полученные линии,
    // а он в конструкторе разбирается со стилем их отображения.
    // На выходе из метода будет полностью готовый график.
    return new MarketDepthGraph(bidsSeries, asksSeries);
}