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
    StonksMainWindow(OrderBookTableModel* orderBookTableModel, QWidget* parent = Q_NULLPTR);

private slots:
    void centerOrderBookTable();
    void insertNewDataAndUpdate();

private:
    Ui::StonksAppClass ui;
    OrderBookTableModel* model;
    QTimer* tmr;

    MarketDepthGraph* marketDepthGraph;
    QChartView* marketDepthView;
    QGridLayout* graphLayout;

    void placeMarketDepthGraph();
    void placeOrderBookTable();

    void updateWindow();
    void updateOrderBookTable();
    void updateMarketDepthGraph();
};
