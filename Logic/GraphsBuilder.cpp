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
    auto iter = asks->begin();
    int prevY = iter->second;
    asksUpLineSeries->append(price, nOrders);
    iter++;
    for (; iter != asks->end(); iter++) {
        asksUpLineSeries->append(price, qreal(prevY));
        asksUpLineSeries->append(price, qreal((iter->second) + prevY));
        prevY += iter->second;
    }

    // И из другого.
    iter = bids->end();
    iter--;
    prevY = iter->second;
    bidsUpLineSeries->append(price, nOrders);
    do {
        iter--;
        bidsUpLineSeries->append(price, qreal(prevY));
        bidsUpLineSeries->append(price, qreal((iter->second) + prevY));
        prevY += iter->second;
    } while (iter != bids->begin());

    // Рисуем дно каждой половины графика.
    iter = bids->end();
    iter--;
    *bidsDownLineSeries << QPointF(price, 0) << QPointF(priceAsQReal(bids->begin()->first), 0);
    iter = asks->end();
    iter--;
    *asksDownLineSeries << QPointF(priceAsQReal(asks->begin()->first), 0) << QPointF(price, 0);

    // Получаем полную форму каждой половины графика, по их кривой сверху и дну снизу.
    QAreaSeries* bidsSeries = new QAreaSeries(bidsUpLineSeries, bidsDownLineSeries);
    QAreaSeries* asksSeries = new QAreaSeries(asksUpLineSeries, asksDownLineSeries);

    // В график запихиваем все эти полученные линии,
    // а он в конструкторе разбирается со стилем их отображения.
    // На выходе из метода будет полностью готовый график.
    return new MarketDepthGraph(bidsSeries, asksSeries);
}