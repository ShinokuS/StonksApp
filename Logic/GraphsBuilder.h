#pragma once

#include "../UI/Graphs/MarketDepthGraph.h"
#include "OrderBook.h"

class GraphsBuilder
{
public:

	static MarketDepthGraph* buildMarketDepthGraph(OrderBook* orderBook);

private:


};