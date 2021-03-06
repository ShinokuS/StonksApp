﻿#include <thread>

#include <QtWidgets/QApplication>

#include "UI\StonksMainWindow.h"
#include "IO/Parser.h"

int main(int argc, char *argv[])
{
    auto parser = new Parser;
    parser->openFile("20200620.deribit.dump");
    parser->setInstrumentName("ETH-PERPETUAL");

    auto orderBook = parser->parsePreDayOrders();

    auto dealsSource = new std::vector<Order>();
    dealsSource->reserve(50000); // 20.06.2020 по ETH-PERPETUAL было 18 тысяч сделок
    parser->setDealsStorage(dealsSource);
    auto dealsModel = new Deals(dealsSource);
    
    auto bot = new BotLogic();
    
    std::thread threadForParsing ([parser]() { parser->parseDaytimeStuff(); });

    QApplication a(argc, argv);
    StonksMainWindow mainWindow(orderBook, dealsModel, bot, parser);
    mainWindow.show();

    return a.exec();
}