#pragma once

#include "Order.h"
#include "Time.h"

#include <QtGlobal>

class Deals
{
public:
	QList<Order*> dealsForPriceGraph;
	double maxPrice;
	double minPrice;

	void addNewDeal(qreal price, qreal quantity, time_t time);
	void randomListOfDeals(unsigned int seed = time(0));

private:
	int firstIntervalTime;
	int timeOfInterval = 5;
};