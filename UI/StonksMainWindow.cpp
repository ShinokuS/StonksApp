#include "StonksMainWindow.h"

#include <QHeaderView>
#include <QTimer>
#include "../IO/Parser.h"

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
}

void StonksMainWindow::insertNewDataAndUpdate() 
{
    Parser::Parse("20200620.deribit.dump", "ETH-PERPETUAL", model);
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
    auto newLinePriceGraph = GraphsBuilder::buildLinePriceGraph(dealsModel);
    priceGraphView->setChart(newLinePriceGraph);
    delete linePriceGraph;
    linePriceGraph = newLinePriceGraph;
    ui.priceGraphWidget->repaint();
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

    // Пока кривой зум, в будущем возможно добавлю нормальный
    //marketDepthView->setRubberBand(QChartView::VerticalRubberBand);
    
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
    linePriceGraph = GraphsBuilder::buildLinePriceGraph(dealsModel);

    priceGraphView = new QChartView(linePriceGraph);
    priceGraphView->setRenderHint(QPainter::Antialiasing);

    priceGraphLayout = new QGridLayout(this);
    priceGraphLayout->addWidget(priceGraphView);
    ui.priceGraphWidget->setLayout(priceGraphLayout);
}