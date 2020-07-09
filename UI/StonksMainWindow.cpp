#include "StonksMainWindow.h"

#include <QtCharts/QChartView>
#include <QHeaderView>
#include <QGridLayout>

using namespace QtCharts;

// parent по умолчанию описан в хэдере (Q_NULLPTR короч)
StonksMainWindow::StonksMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.centerButton, SIGNAL(clicked()), this, SLOT(centerOrderBookTable()));
    model = new OrderBookTableModel(this);
    model->fillRandomly();

    this->placeMarketDepthGraph();
    this->placeOrderBookTable();
}

void StonksMainWindow::centerOrderBookTable()
{
    ui.tableView->scrollTo(StonksMainWindow::model->index(model->returnCenterIndex() - 2, 0), QAbstractItemView::PositionAtCenter);
}

void StonksMainWindow::placeMarketDepthGraph()
{
    auto graph = GraphsBuilder::buildMarketDepthGraph(model);

    QChartView* MarketDepthView = new QChartView(graph);
    MarketDepthView->setRenderHint(QPainter::Antialiasing);

    // Пока кривой зум, в будущем возможно добавлю нормальный
    //MarketDepthView->setRubberBand(QChartView::VerticalRubberBand);
    
    QGridLayout* graphLayout = new QGridLayout(this);
    graphLayout->addWidget(MarketDepthView);
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