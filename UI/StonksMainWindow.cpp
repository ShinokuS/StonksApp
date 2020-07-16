﻿#include "StonksMainWindow.h"

#include <QHeaderView>
#include <QTimer>
#include <algorithm>

using namespace QtCharts;

// parent по умолчанию описан в хэдере (Q_NULLPTR короч)
StonksMainWindow::StonksMainWindow(OrderBookTableModel* orderBookTableModel, Deals* deals, QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.centerButton, SIGNAL(clicked()), this, SLOT(centerOrderBookTable()));
    
    model = orderBookTableModel;
    model->setParent(this);

    dealsModel = deals;
    model->deals = dealsModel;

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
    int firstDayTime = (int(GraphsBuilder::getTimeForLinePriceGraph(dealsModel).first()) / 86400) * 86400 - 10800;
    int lastDayTime = firstDayTime + 86400;
    priceGraph->xAxis->setTickStep((newRange.size() <= 10800) ? 600 : 7200);
    QCPRange boundedRange = newRange;
  
    if (boundedRange.lower < firstDayTime || boundedRange.upper>lastDayTime) {
        boundedRange.lower = firstDayTime;
        boundedRange.upper = lastDayTime;
    }
   
    priceGraph->xAxis->setRange(boundedRange);

}

void StonksMainWindow::insertNewDataAndUpdate() 
{
    model->addAsk(rand() % 100 + 30, rand() % 5000 + 1000);
    model->addBid(rand() % 100 + 30, rand() % 5000 + 1000);
    
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
    model->updateTable();
    this->centerOrderBookTable();
    ui.tableView->repaint();
}

void StonksMainWindow::updateMarketDepthGraph()
{
    auto newMarketDepthGraph = GraphsBuilder::buildMarketDepthGraph(model);
    marketDepthView->setChart(newMarketDepthGraph);
    delete marketDepthGraph;
    marketDepthGraph = newMarketDepthGraph;
    ui.graphWidget->repaint();
}

void StonksMainWindow::updatePriceGraph()
{
    priceGraph->graph()->clearData();
    if (!GraphsBuilder::getTimeForLinePriceGraph(dealsModel).empty()) {
        if (isFirstDeal) {
            int firstDayTime = (int(GraphsBuilder::getTimeForLinePriceGraph(dealsModel).first()) / 86400) * 86400 - 10800;
            int lastDayTime = firstDayTime + 86400;
            priceGraph->xAxis->setRange(firstDayTime, lastDayTime);
            isFirstDeal = false;
        }
        priceGraph->graph()->clearData();
        priceGraph->graph()->setData(GraphsBuilder::getTimeForLinePriceGraph(dealsModel), GraphsBuilder::getPriceForLinePriceGraph(dealsModel));
        priceGraph->yAxis->setRange(dealsModel->dealsForLineGraph.last()->price, dealsModel->maxPrice, Qt::AlignBottom);
        ui.OHLC->setText("Close: "+ QString::number(dealsModel->dealsForLineGraph.last()->price));
    }
    priceGraph->replot();
}

void StonksMainWindow::centerOrderBookTable()
{
    ui.tableView->scrollTo(StonksMainWindow::model->index(model->centerIndex - 2, 0), QAbstractItemView::PositionAtCenter);
}

void StonksMainWindow::placeMarketDepthGraph()
{
    marketDepthGraph = GraphsBuilder::buildMarketDepthGraph(model);

    marketDepthView = new QChartView(marketDepthGraph);
    marketDepthView->setRenderHint(QPainter::Antialiasing);

    graphLayout = new QGridLayout(this);
    graphLayout->addWidget(marketDepthView);
    ui.graphWidget->setLayout(graphLayout);
}

void StonksMainWindow::placeOrderBookTable()
{
    ui.tableView->setModel(model);

    ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui.tableView->verticalHeader()->hide();

    StonksMainWindow::centerOrderBookTable();
}

void StonksMainWindow::placePriceGraph()
{
    priceGraph = new PriceGraph(GraphsBuilder::getTimeForLinePriceGraph(dealsModel), GraphsBuilder::getPriceForLinePriceGraph(dealsModel));
    if (GraphsBuilder::getTimeForLinePriceGraph(dealsModel).empty()) {
        isFirstDeal = true;
    }
    else {
        int firstDayTime = (int(GraphsBuilder::getTimeForLinePriceGraph(dealsModel).first()) / 86400) * 86400 - 10800;
        int lastDayTime = firstDayTime + 86400;
        priceGraph->xAxis->setRange(firstDayTime, lastDayTime);
        isFirstDeal = false;
    }
    priceGraphLayout = new QGridLayout(this);
    priceGraphLayout->addWidget(priceGraph);
    ui.priceGraphWidget->setLayout(priceGraphLayout);
}