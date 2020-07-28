#pragma once

#include "Order.h"
#include "Time.h"

#include <QtGlobal>

#include <vector>

class Deals
{
public:
	QList<Order*> dealsForPriceGraph;

	Deals(std::vector<Order>* dealsSource);

	bool canLoadNextDealFromSource();
	void loadNextDealFromSource();
	bool canLoadMoreFromTheSameFrame();
	Order* getLastDeal();

private:
	std::vector<Order>* dealsSource;
	int activeDealIndexInSource;
};