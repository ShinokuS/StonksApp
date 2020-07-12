#pragma once

#include "../UI/Graphs/MarketDepthGraph.h"
#include "OrderBookTableModel.h"
#include "../UI/Graphs/PriceGraph.h"
#include "Deals.h"

class GraphsBuilder
{
public:
	static MarketDepthGraph* buildMarketDepthGraph(OrderBookTableModel* orderBook);
	static LinePriceGraph* buildLinePriceGraph(Deals* deals);
};