#include "GraphsBuilder.h"
#include "OrderBook.h"

#include <QtGlobal>
#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>

using namespace QtCharts;

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
    asksUpLineSeries->append(qreal(iter->first), qreal(iter->second));
    iter++;
    for (; iter != asks->end(); iter++) {
        asksUpLineSeries->append(qreal(iter->first), qreal(prevY));
        asksUpLineSeries->append(qreal(iter->first), qreal((iter->second) + prevY));
        prevY += iter->second;
    }

    // И из другого.
    iter = bids->end();
    iter--;
    prevY = iter->second;
    bidsUpLineSeries->append(qreal(iter->first), qreal(iter->second));
    do {
        iter--;
        bidsUpLineSeries->append(qreal(iter->first), qreal(prevY));
        bidsUpLineSeries->append(qreal(iter->first), qreal((iter->second) + prevY));
        prevY += iter->second;
    } while (iter != bids->begin());

    // Рисуем дно каждой половины графика.
    iter = bids->end();
    iter--;
    *bidsDownLineSeries << QPointF(qreal(iter->first), 0) << QPointF(qreal(bids->begin()->first), 0);
    iter = asks->end();
    iter--;
    *asksDownLineSeries << QPointF(qreal(asks->begin()->first), 0) << QPointF(qreal(iter->first), 0);

    // Получаем полную форму каждой половины графика, по их кривой сверху и дну снизу.
    QAreaSeries* bidsSeries = new QAreaSeries(bidsUpLineSeries, bidsDownLineSeries);
    QAreaSeries* asksSeries = new QAreaSeries(asksUpLineSeries, asksDownLineSeries);

    // В график запихиваем все эти полученные линии,
    // а он в конструкторе разбирается со стилем их отображения.
    // На выходе из метода будет полностью готовый график.
    return new MarketDepthGraph(bidsSeries, asksSeries);
}