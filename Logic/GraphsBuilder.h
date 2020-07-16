#pragma once

#include "../UI/Graphs/MarketDepthGraph.h"
#include "OrderBookTableModel.h"
#include "../UI/Graphs/PriceGraph.h"
#include "Deals.h"

class GraphsBuilder
{
public:
	MarketDepthGraph* buildMarketDepthGraph(OrderBookTableModel* orderBook);

	PriceGraph* buildPriceGraph(Deals* deals);
	void update(PriceGraph* priceGraph, Deals* dealsModel);

	QVector<double> getTimeForLinePriceGraph(Deals* deals);
	QVector<double> getPriceForLinePriceGraph(Deals* deals);

	bool isFirstDeal;
};