#pragma once

#include "../UI/Graphs/MarketDepthGraph.h"
#include "OrderBookTableModel.h"

class GraphsBuilder
{
public:
	static MarketDepthGraph* buildMarketDepthGraph(OrderBookTableModel* orderBook);
};