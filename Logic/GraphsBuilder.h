#pragma once

#include "../UI/Graphs/MarketDepthGraph.h"
#include "../UI/Graphs/PriceGraph.h"
#include "OrderBook.h"
#include "BotLogic.h"
#include "Deals.h"
#include "Time.h"

class GraphsBuilder
{
public:
	MarketDepthGraph* buildMarketDepthGraph(OrderBook* orderBook);

	PriceGraph* buildPriceGraph(Deals* deals);
	void updatePriceGraph(PriceGraph* priceGraph, BotLogic* bot);
	void updateMarketDepthGraph(MarketDepthGraph* marketDepthGraph);

	QVector<double> getTimeForPriceGraph();
	QVector<double> getPriceForPriceGraph();

	QVector<double> getPriceAskForMarketDepthGraph();
	QVector<double> getPriceBidForMarketDepthGraph();
	QVector<double> getQuantityAskForMarketDepthGraph();
	QVector<double> getQuantityBidForMarketDepthGraph();

private:
	double minPrice=0;
	double maxPrice=0;
	Deals* dealsModel;
	OrderBook* orderBook;
	bool isFirstDeal;
	bool isFirstOrder;
};