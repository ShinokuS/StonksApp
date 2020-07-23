#pragma once

#include "../../Logic/QCustomPlot/qcustomplot.h"
#include "PriceGraph.h"
#include "../../Logic/Order.h"

class BotGraph : public QCustomPlot
{
public:
	BotGraph(QVector <double> timeBuy, QVector <double> priceBuy,
		QVector <double> timeSell, QVector <double> priceSell, PriceGraph* priceGraph);
};
