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

    ui.tableView->setModel(model);
    int centerIndex = model->rowCount() / 2 - 1;
    ui.tableView->scrollTo(model->index(centerIndex,0), QAbstractItemView::PositionAtCenter);
}
