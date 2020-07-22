﻿#include <QHeaderView>
#include <QTimer>
#include <algorithm>

#include "StonksMainWindow.h"
#include "../IO/Parser.h"

using namespace QtCharts;

// parent по умолчанию описан в хэдере (Q_NULLPTR короч)
StonksMainWindow::StonksMainWindow(OrderBookTableModel* orderBookTableModel ,SmallOrderBookTableModel* smallOrderBookTableModel,
                                Deals* deals, QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.centerButton, SIGNAL(clicked()), this, SLOT(centerOrderBookTable()));
    
    
    allOrders = orderBookTableModel;
    visibleOrders = smallOrderBookTableModel;

    dealsModel = deals;
    //model->deals = dealsModel;

    graphsBuilder = new GraphsBuilder;

    // Я бы хотел это распараллелить, но Qt не даёт.
    this->placeMarketDepthGraph();
    this->placeOrderBookTable();
    this->placePriceGraph();

    // таймер, чтобы каждую секунду вбрасывать новые ордеры и обновлять окно
    tmr = new QTimer();
    tmr->setInterval(1000);
    connect(tmr, SIGNAL(timeout()), this, SLOT(insertNewDataAndUpdate()));
    tmr->start();

    connect(priceGraph->xAxis, SIGNAL(rangeChanged(QCPRange)),
        this, SLOT(slotRangeChanged(QCPRange)));
}

void StonksMainWindow::slotRangeChanged(const QCPRange& newRange)
{
    int firstDayTime = (int(graphsBuilder->getTimeForPriceGraph().first()) / Time::DAY)
                        * Time::DAY - Time::THREE_HOURS;
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
    Parser::ParseDaytimeOrders("20200620.deribit.dump", "ETH-PERPETUAL", allOrders);
    updateWindow();
}

void StonksMainWindow::updateWindow()
{
    // Я бы хотел это распараллелить, но Qt не даёт.
    updatePriceGraph();
    updateMarketDepthGraph();
    updateOrderBookTable();
}

void StonksMainWindow::updateOrderBookTable()
{
    //model->updateTable();
    //this->centerOrderBookTable();
    //ui.tableView->repaint();
}

void StonksMainWindow::updateMarketDepthGraph()
{
    auto newMarketDepthGraph = graphsBuilder->buildMarketDepthGraph(allOrders);
    marketDepthView->setChart(newMarketDepthGraph);
    delete marketDepthGraph;
    marketDepthGraph = newMarketDepthGraph;
    ui.graphWidget->repaint();
}

void StonksMainWindow::updatePriceGraph()
{
    graphsBuilder->update(priceGraph);
}

void StonksMainWindow::centerOrderBookTable()
{
    /*
    ui.tableView->scrollTo(model->index(model->centerIndex - 2, 0),
                            QAbstractItemView::PositionAtCenter);
    */
}

void StonksMainWindow::placeMarketDepthGraph()
{
    marketDepthGraph = graphsBuilder->buildMarketDepthGraph(allOrders);

    marketDepthView = new QChartView(marketDepthGraph);
    marketDepthView->setRenderHint(QPainter::Antialiasing);

    graphLayout = new QGridLayout(this);
    graphLayout->addWidget(marketDepthView);
    ui.graphWidget->setLayout(graphLayout);
}

void StonksMainWindow::placeOrderBookTable()
{
    ui.tableView->setModel(visibleOrders);

    ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui.tableView->verticalHeader()->hide();

    //StonksMainWindow::centerOrderBookTable();
}

void StonksMainWindow::placePriceGraph()
{
    priceGraph = graphsBuilder->buildPriceGraph(dealsModel);
    priceGraphLayout = new QGridLayout(this);
    priceGraphLayout->addWidget(priceGraph);
    ui.priceGraphWidget->setLayout(priceGraphLayout);
}