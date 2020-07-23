#include <QtWidgets/QApplication>

#include "UI\StonksMainWindow.h"
#include "IO/Parser.h"

int main(int argc, char *argv[])
{
    auto dealsSource = Deals::makeRandomListOfDeals();
    auto dealsModel = new Deals(dealsSource);
    auto bot = new BotLogic();
    auto orderBook = Parser::parsePreDayOrders("20200620.deribit.dump","ETH-PERPETUAL");
    
    QApplication a(argc, argv);
    StonksMainWindow mainWindow(orderBook, dealsModel, bot);
    mainWindow.show();

    return a.exec();
}