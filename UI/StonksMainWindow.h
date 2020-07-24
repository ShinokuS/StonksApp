#pragma once

#include <QtWidgets/QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <QtCharts>

#include "ui_StonksMainWindow.h"
#include "../Logic/GraphsBuilder.h"
#include "../Logic/OrderBook.h"
#include "../Logic/SmallOrderBookTableModel.h"
#include "../Logic/BotLogic.h"
#include "Graphs/MarketDepthGraph.h"
#include "Graphs/BotGraph.h"
#include "../IO/Parser.h"

class StonksMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    StonksMainWindow(OrderBook* orderBookTableModel, Deals* deals, 
                    BotLogic* bot, Parser* parser, QWidget* parent = Q_NULLPTR);

private slots:
    void insertNewDataAndUpdate();
    void slotRangeChanged(const QCPRange& newRange);

private:
    Ui::StonksAppClass ui;
    BotLogic* botLogic;
    BotGraph* botGraph;

    SmallOrderBookTableModel* visibleOrders;
    OrderBook* orderBook;
    Deals* dealsModel;
    Parser* parser;
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
