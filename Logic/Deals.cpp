#include "Deals.h"

void Deals::addNewDeal(qreal price, qreal quantity, time_t time)
{
	auto newDeal = new Order{ price, quantity, 0, time };

	if (minPrice > price || minPrice==0) {
		minPrice = price;
	}
	if (maxPrice < price) {
		maxPrice = price;
	}
	if (!firstIntervalTime) {
		dealsForPriceGraph.append(newDeal);
		firstIntervalTime = newDeal->time;
	}
	else {
		if (newDeal->time - firstIntervalTime < timeOfInterval) {
			delete dealsForPriceGraph.last();
			dealsForPriceGraph.last() = newDeal;
		}
		else {
			dealsForPriceGraph.append(newDeal);
			firstIntervalTime += timeOfInterval;
		}
	}
}