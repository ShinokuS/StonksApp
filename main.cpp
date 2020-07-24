#include <QtWidgets/QApplication>

#include "UI\StonksMainWindow.h"
#include "IO/Parser.h"

int main(int argc, char *argv[])
{
    auto parser = new Parser;
    parser->openFile("20200620.deribit.dump");
    parser->setInstrumentName("ETH-PERPETUAL");

    auto dealsSource = new std::vector<Order*>();
    dealsSource->reserve(1000000);
    parser->setDealsStorage(dealsSource);
    
    auto dealsModel = new Deals(dealsSource);
    auto bot = new BotLogic();
    auto orderBook = parser->parsePreDayOrders();
    
    QApplication a(argc, argv);
    StonksMainWindow mainWindow(orderBook, dealsModel, bot, parser);
    mainWindow.show();

    return a.exec();
}