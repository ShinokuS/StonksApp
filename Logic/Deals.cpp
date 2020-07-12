#include "Deals.h"

void Deals::addNewDeal(qreal price, qreal quantity, time_t time)
{
	auto newDeal = new Order{ price, quantity, 0, time };
	deals.append(newDeal);
}

void Deals::addDealForLineGraph()
{
	auto newDeal = deals.last();
	int timeOfInterval = 60;
	if (!firstTimeOfInterval) {
		dealsForLineGraph.append(newDeal);
		firstTimeOfInterval = newDeal->time;
	}
	else {
		if (newDeal->time - firstTimeOfInterval < timeOfInterval) {
			dealsForLineGraph.replace(dealsForLineGraph.indexOf(dealsForLineGraph.last()), newDeal);
		}
		else {
			dealsForLineGraph.append(newDeal);
			firstTimeOfInterval += timeOfInterval;
		}
	}
}