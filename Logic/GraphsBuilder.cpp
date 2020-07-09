#include "GraphsBuilder.h"

#include <QtGlobal>
#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>

using namespace QtCharts;

MarketDepthGraph* GraphsBuilder::buildMarketDepthGraph(OrderBookTableModel* orderBook) {

    // Объявляем серии для хранения точек для линий графика.
    QLineSeries* bidsUpLineSeries = new QLineSeries();
    QLineSeries* bidsDownLineSeries = new QLineSeries();
    QLineSeries* asksUpLineSeries = new QLineSeries();
    QLineSeries* asksDownLineSeries = new QLineSeries();

    // Обрабатываем данные с модели
    auto iter = orderBook->rows.end();
    do iter--; while (!iter->askMarker);
    auto centralIter = iter;
    qreal prevY = iter->quantity;
    asksUpLineSeries->append(prevY, iter->price);
    do {
        iter--;
        asksUpLineSeries->append(prevY, iter->price);
        asksUpLineSeries->append(prevY + iter->quantity, iter->price);
        prevY += iter->quantity;
    } while (iter!=orderBook->rows.begin());
    *asksDownLineSeries << QPointF(0, centralIter->price) << QPointF(0, iter->price);

    iter = centralIter++;
    prevY = iter->quantity;
    iter++;
    for (; iter != orderBook->rows.end(); iter++) {
        bidsUpLineSeries->append(prevY, iter->price);
        bidsUpLineSeries->append(prevY + iter->quantity, iter->price);
        prevY += iter->quantity;
    }
    iter--;
    *bidsDownLineSeries << QPointF(0, centralIter->price) << QPointF(0, iter->price);

    // Получаем полную форму каждой половины графика по кривой сверху и дну снизу.
    QAreaSeries* bidsSeries = new QAreaSeries(bidsUpLineSeries, bidsDownLineSeries);
    QAreaSeries* asksSeries = new QAreaSeries(asksUpLineSeries, asksDownLineSeries);

    // В график запихиваем все эти полученные линии,
    // а он в конструкторе разбирается со стилем их отображения.
    // На выходе из метода будет полностью готовый график.
    return new MarketDepthGraph(bidsSeries, asksSeries);
}