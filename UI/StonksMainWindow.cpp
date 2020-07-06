#include "StonksMainWindow.h"

#include <QtCharts/QChartView>

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QGridLayout>

using namespace QtCharts;

StonksMainWindow::StonksMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

void StonksMainWindow::placeMarketDepthGraph()
{
    auto orderBook = OrderBook::getTestOrderBook();
    auto graph = GraphsBuilder::buildMarketDepthGraph(orderBook);

    QChartView* MarketDepthView = new QChartView(graph);
    MarketDepthView->setRenderHint(QPainter::Antialiasing);
    
    QGridLayout* graphLayout = new QGridLayout(this);
    graphLayout->addWidget(MarketDepthView);
    ui.graphWidget->setLayout(graphLayout);

}

void StonksMainWindow::placeOrderBookTable()
{
    auto model = new OrderBookTableModel(this);
    auto orderBook = OrderBook::getTestOrderBook();
    model->initOrderBookTableStruct(orderBook);
    ui.tableView->setModel(model);
    

    ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui.tableView->verticalHeader()->hide();
    ui.tableView->scrollTo(model->index(model->returnCenterIndex()-1,0), QAbstractItemView::PositionAtCenter);
}
