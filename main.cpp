#include "UI\StonksMainWindow.h"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    auto orderBook = OrderBook::getTestOrderBook();

    QApplication a(argc, argv);
    StonksMainWindow mainWindow;
    mainWindow.placeMarketDepthGraph(orderBook);
    mainWindow.placeOrderBookTable();

    mainWindow.show();
    return a.exec();
}
