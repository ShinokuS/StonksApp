#pragma once

#include "../UI/Graphs/MarketDepthGraph.h"
#include "../UI/Graphs/PriceGraph.h"
#include "OrderBook.h"
#include "Deals.h"
#include "Time.h"

class GraphsBuilder
{
public:
	MarketDepthGraph* buildMarketDepthGraph(OrderBook* orderBook);

	PriceGraph* buildPriceGraph(Deals* deals);
	void update(PriceGraph* priceGraph);

	QVector<double> getTimeForPriceGraph();
	QVector<double> getPriceForPriceGraph();

private:
	Deals* dealsModel;
	bool isFirstDeal;
};