﻿#include <QHeaderView>
#include <QTimer>
#include <algorithm>

#include "StonksMainWindow.h"
#include "../IO/Parser.h"

using namespace QtCharts;

// parent по умолчанию описан в хэдере (Q_NULLPTR короч)
StonksMainWindow::StonksMainWindow(OrderBook* orderBook, Deals* deals, BotLogic* bot, Parser* parser, QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    
    this->orderBook = orderBook;

    dealsModel = deals;
    botLogic = bot;
    this->parser = parser;

    graphsBuilder = new GraphsBuilder;

    // Я бы хотел это распараллелить, но Qt не даёт.
    this->placeMarketDepthGraph();
    this->placeOrderBookTable();
    this->placePriceGraph();

    // Этот таймер оставлен как костыль вместо цикла по сделкам.
    // Так приложение работает приятнее, без подвисаний.
    // Можете сравнить с веткой "замена-таймера-на-цикл".
    tmr = new QTimer();
    tmr->setInterval(1);
    connect(tmr, SIGNAL(timeout()), this, SLOT(insertNewDataAndUpdate()));
    tmr->start();

    connect(priceGraph->xAxis, SIGNAL(rangeChanged(QCPRange)),
        this, SLOT(slotRangeChanged(QCPRange)));
}

void StonksMainWindow::slotRangeChanged(const QCPRange& newRange)
{
    int firstDayTime = graphsBuilder->getTimeForPriceGraph().first();
    int lastDayTime = firstDayTime + Time::DAY;

    if (newRange.size() < Time::ONE_MINUTE) {
        priceGraph->xAxis->setDateTimeFormat("hh:mm:ss");
        priceGraph->xAxis->setTickStep(Time::TEN_SECONDS);
    }
    else if (newRange.size()< Time::FIVE_MINUTES) {
        priceGraph->xAxis->setDateTimeFormat("hh:mm");
        priceGraph->xAxis->setTickStep(Time::ONE_MINUTE);
    }
    else if (newRange.size() < Time::HALF_OF_HOUR) {
        priceGraph->xAxis->setTickStep(Time::FIVE_MINUTES);
    }
    else if (newRange.size() < Time::THREE_HOURS) {
        priceGraph->xAxis->setTickStep(Time::HALF_OF_HOUR);
    }
    else {
        priceGraph->xAxis->setTickStep(Time::THREE_HOURS);
    }

    QCPRange boundedRange = newRange;
    if (boundedRange.lower < firstDayTime) {
        boundedRange.lower = firstDayTime;
    }
    if (boundedRange.upper>lastDayTime) {
        boundedRange.upper = lastDayTime;
    }
   
    priceGraph->xAxis->setRange(boundedRange);

}

void StonksMainWindow::insertNewDataAndUpdate() 
{
    if (dealsModel->canLoadNextDealFromSource()) {

        // Считываем все ордеры между прошлой сделкой и новой
        while (orderBook->canLoadNextOrderFromSource()) {
            orderBook->loadNextOrderFromSource();
        }
        
        // Считываем новую сделку
        dealsModel->loadNextDealFromSource();
        botLogic->reactAtNewDeal(dealsModel->getLastDeal());

        // Считываем все остальные сделки из пачки за тот же момент:
        while (dealsModel->canLoadMoreFromTheSameFrame()) {
            dealsModel->loadNextDealFromSource();
            botLogic->reactAtNewDeal(dealsModel->getLastDeal());
        }

        updateWindow();
    }
    else if (parser->hasFinished) {
        // День закончен
        tmr->stop();

        if (botLogic->botThingsQuantity > 0) {
            botLogic->sellAfter(dealsModel->getLastDeal());
        }
        updateWindow();
        showDayResultsOnLabel();
    }

    // В противном случае просто ничего не делаем, пропуская этот тик
    // в ожидании, пока парсер подпарсит дальше.
}

void StonksMainWindow::updateWindow()
{
    // Я бы хотел это распараллелить, но Qt не даёт.
    updatePriceGraph();
    updateMarketDepthGraph();
    updateOrderBookTable();

    ui.label->setText("Balance: " + QString::number(botLogic->botBalance) +
        "\nThings: " + QString::number(botLogic->botThingsQuantity));
}

void StonksMainWindow::updateOrderBookTable()
{
    visibleOrders->changeData(orderBook);
    visibleOrders->updateTable();
    ui.tableView->repaint();
}

void StonksMainWindow::updateMarketDepthGraph()
{
    graphsBuilder->updateMarketDepthGraph(marketDepthGraph);
}

void StonksMainWindow::updatePriceGraph()
{
    graphsBuilder->updatePriceGraph(priceGraph, botLogic);
}

void StonksMainWindow::placeMarketDepthGraph()
{
    marketDepthGraph = graphsBuilder->buildMarketDepthGraph(orderBook);

    graphLayout = new QGridLayout(this);
    graphLayout->addWidget(marketDepthGraph);
    ui.graphWidget->setLayout(graphLayout);
}

void StonksMainWindow::placeOrderBookTable()
{
    visibleOrders = new SmallOrderBookTableModel();
    visibleOrders->changeData(orderBook);

    ui.tableView->setModel(visibleOrders);

    ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui.tableView->verticalHeader()->hide();
}

void StonksMainWindow::placePriceGraph()
{
    priceGraph = graphsBuilder->buildPriceGraph(dealsModel);
    botGraph = new BotGraph(botLogic->timeBuy, botLogic->priceBuy, 
                            botLogic->timeSell, botLogic->priceSell, priceGraph);
    priceGraphLayout = new QGridLayout(this);
    priceGraphLayout->addWidget(priceGraph);
    ui.priceGraphWidget->setLayout(priceGraphLayout);
}

void StonksMainWindow::showDayResultsOnLabel()
{
    ui.label->setFixedHeight(121);
    ui.label->setText(QString("DAY ENDED") +
        "\n\nBalance: " + QString::number(botLogic->botBalance) +
        "\nThings: " + QString::number(botLogic->botThingsQuantity) +
        "\n\nStart balance: " + QString::number(botLogic->START_BALANCE) +
        "\nDaily profit: " + QString::number(
            ((botLogic->botBalance / botLogic->START_BALANCE) - 1) * 100) + " %");
}