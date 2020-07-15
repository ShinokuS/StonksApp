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
		dealsForLineGraph.append(newDeal);
		firstIntervalTime = newDeal->time;
	}
	else {
		if (newDeal->time - firstIntervalTime < timeOfInterval) {
			delete dealsForLineGraph.last();
			dealsForLineGraph.last() = newDeal;
		}
		else {
			dealsForLineGraph.append(newDeal);
			firstIntervalTime += timeOfInterval;
		}
	}
}