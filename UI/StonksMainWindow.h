#pragma once

#include <QtWidgets/QMainWindow>


#include "ui_StonksApp.h"
#include "../Logic/OrderBook.h"
#include "../Logic/GraphsBuilder.h"
#include "Graphs/MarketDepthGraph.h"

class StonksMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    StonksMainWindow(QWidget* parent = Q_NULLPTR);

    void placeMarketDepthGraph();

private:
    Ui::StonksAppClass ui;

};
