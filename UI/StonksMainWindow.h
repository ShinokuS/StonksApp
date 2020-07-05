#pragma once

#include <QtWidgets/QMainWindow>

#include "ui_StonksMainWindow.h"
#include "../Logic/OrderBook.h"
#include "../Logic/GraphsBuilder.h"
#include "../Logic/OrderBookTableModel.h"
#include "Graphs/MarketDepthGraph.h"


class StonksMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    StonksMainWindow(QWidget* parent = Q_NULLPTR);

    void placeMarketDepthGraph();
    void placeOrderBookTable();

private:
    Ui::StonksAppClass ui;
};
