#pragma once

#include "../UI/Graphs/MarketDepthGraph.h"
#include "OrderBookTableModel.h"
#include "../UI/Graphs/PriceGraph.h"
#include "Deals.h"

class GraphsBuilder
{
public:
	static MarketDepthGraph* buildMarketDepthGraph(OrderBookTableModel* orderBook);
	static QVector<double> getTimeForLinePriceGraph(Deals* deals);
	static QVector<double> getPriceForLinePriceGraph(Deals* deals);
};