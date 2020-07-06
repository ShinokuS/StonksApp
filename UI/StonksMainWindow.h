#pragma once

#include <QtWidgets/QMainWindow>
#include <QPushButton>

#include "ui_StonksMainWindow.h"
#include "../Logic/OrderBook.h"
#include "../Logic/GraphsBuilder.h"
#include "../Logic/OrderBookTableModel.h"
#include "Graphs/MarketDepthGraph.h"


class StonksMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    StonksMainWindow(OrderBook* relatedOrderBook, QWidget* parent = Q_NULLPTR);
    
private slots:
    void centerOrderBookTable();

private:
    Ui::StonksAppClass ui;
    OrderBook* relatedOrderBook;
    OrderBookTableModel* model;

    void placeMarketDepthGraph();
    void placeOrderBookTable();

};
