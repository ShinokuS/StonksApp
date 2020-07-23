#include <QtWidgets/QApplication>

#include "UI\StonksMainWindow.h"
#include "IO/Parser.h"

int main(int argc, char *argv[])
{
    auto deals = new Deals();
    deals->randomListOfDeals();
    auto bot = new BotLogic(deals->dealsForPriceGraph);
    auto orderBook = Parser::parsePreDayOrders("20200620.deribit.dump","ETH-PERPETUAL");
    
    QApplication a(argc, argv);
    StonksMainWindow mainWindow(orderBook, deals, bot);
    mainWindow.show();

    return a.exec();
}