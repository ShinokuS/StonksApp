#include <QtWidgets/QApplication>

#include "UI\StonksMainWindow.h"
#include "IO/Parser.h"

int main(int argc, char *argv[])
{
    Parser::openFile("20200620.deribit.dump");
    auto dealsSource = new std::vector<Order*>();
    dealsSource->reserve(1000000);
    Parser::setDealsStorage(dealsSource);
    Parser::ParseDaytimeDeal("ETH-PERPETUAL");
    auto dealsModel = new Deals(dealsSource);
    auto bot = new BotLogic();
    auto orderBook = Parser::parsePreDayOrders("ETH-PERPETUAL");
    
    QApplication a(argc, argv);
    StonksMainWindow mainWindow(orderBook, dealsModel, bot);
    mainWindow.show();

    return a.exec();
}