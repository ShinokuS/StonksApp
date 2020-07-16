#pragma once

#include <QtWidgets/QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <QtCharts>

#include "ui_StonksMainWindow.h"
#include "../Logic/GraphsBuilder.h"
#include "../Logic/OrderBookTableModel.h"
#include "Graphs/MarketDepthGraph.h"


class StonksMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    StonksMainWindow(OrderBookTableModel* orderBookTableModel, Deals* deals, QWidget* parent = Q_NULLPTR);

private slots:
    void centerOrderBookTable();
    void insertNewDataAndUpdate();
    void slotRangeChanged(const QCPRange& newRange);

private:
    Ui::StonksAppClass ui;
    OrderBookTableModel* model;
    Deals* dealsModel;
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
