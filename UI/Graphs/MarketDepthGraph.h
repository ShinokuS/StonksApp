#pragma once

#include "../../Logic/QCustomPlot/qcustomplot.h"

class MarketDepthGraph : public QCustomPlot
{
public:
	MarketDepthGraph(QVector <double> priceAsk, QVector <double> quantityAsk,
		QVector <double> priceBid, QVector <double> quantityBid);
};