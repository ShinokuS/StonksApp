#include <QtWidgets/QApplication>

#include "UI\StonksMainWindow.h"
#include "IO/Parser.h"

int main(int argc, char *argv[])
{
    auto deals = new Deals();
    auto visibleOrders = new SmallOrderBookTableModel();
    auto orderBook = Parser::parsePreDayOrders("20200620.deribit.dump","ETH-PERPETUAL");
    
    QApplication a(argc, argv);
    StonksMainWindow mainWindow(orderBook, visibleOrders, deals);
    mainWindow.show();

    return a.exec();
}