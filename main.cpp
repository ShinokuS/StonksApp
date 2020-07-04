#include "UI\StonksApp.h"
#include "Logic/OrderBook.h"
#include "Logic/GraphsBuilder.h"
#include "UI/Graphs/MarketDepthGraph.h"

#include <QtCharts/QChartView>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
using namespace QtCharts;

int main(int argc, char *argv[])
{
    auto orderBook = OrderBook::getTestOrderBook();
    auto graph = GraphsBuilder::buildMarketDepthGraph(orderBook);

    QChartView* MarketDepthView = new QChartView(graph);
    MarketDepthView->setRenderHint(QPainter::Antialiasing);

    QApplication a(argc, argv);
    StonksApp w;
    w.setCentralWidget(MarketDepthView);
    w.resize(400, 300);
    w.show();
    return a.exec();
}
