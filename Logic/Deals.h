#pragma once

#include "Order.h"
#include "Time.h"

#include <QtGlobal>

#include <vector>

class Deals
{
public:
	QList<Order*> dealsForPriceGraph;
	double maxPrice;
	double minPrice;

	Deals(std::vector<Order*>* dealsSource);

	static std::vector<Order*>* makeRandomListOfDeals(unsigned int seed = time(0));

	bool canLoadNextDealFromSource();
	void loadNextDealFromSource();
	bool canLoadMoreFromTheSameFrame();
	Order* getLastDeal();

private:
	std::vector<Order*>* dealsSource;
	int activeDealIndexInSource;
	int firstIntervalTime;
	int timeOfInterval = 5;

	void addNewDeal(Order* deal);
};