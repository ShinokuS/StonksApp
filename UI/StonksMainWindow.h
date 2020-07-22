#pragma once

#include <QtWidgets/QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <QtCharts>

#include "ui_StonksMainWindow.h"
#include "../Logic/GraphsBuilder.h"
#include "../Logic/OrderBook.h"
#include "../Logic/SmallOrderBookTableModel.h"
#include "Graphs/MarketDepthGraph.h"


class StonksMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    StonksMainWindow(OrderBook* orderBookTableModel, SmallOrderBookTableModel* smallOrderBookTableModel, Deals* deals,
                    QWidget* parent = Q_NULLPTR);

private slots:
    void insertNewDataAndUpdate();
    void slotRangeChanged(const QCPRange& newRange);

private:
    Ui::StonksAppClass ui;
    SmallOrderBookTableModel* visibleOrders;
    OrderBook* allOrders;
    Deals* dealsModel;
    GraphsBuilder* graphsBuilder;
    QTimer* tmr;

    MarketDepthGraph* marketDepthGraph;
    QChartView* marketDepthView;
    QGridLayout* graphLayout;

    PriceGraph* priceGraph;
    QChartView* priceGraphView;
    QGridLayout* priceGraphLayout;

    void placeMarketDepthGraph();
    void placePriceGraph();
    void placeOrderBookTable();

    void updateWindow();
    void updateOrderBookTable();
    void updateMarketDepthGraph();
    void updatePriceGraph();
};
