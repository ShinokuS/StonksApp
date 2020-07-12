﻿#include "Deals.h"

void Deals::addNewDeal(qreal price, qreal quantity, time_t time)
{
	auto newDeal = new Order{ price, quantity, 0, time };

	addDealForLineGraph(newDeal);
}

void Deals::addDealForLineGraph(Order* newDeal)
{
	int timeOfInterval = 5;
	if (!firstIntervalTime) {
		dealsForLineGraph.append(newDeal);
		firstIntervalTime = newDeal->time;
	}
	else {
		if (newDeal->time - firstIntervalTime < timeOfInterval) {
			dealsForLineGraph.replace(dealsForLineGraph.indexOf(dealsForLineGraph.last()), newDeal);
		}
		else {
			dealsForLineGraph.append(newDeal);
			firstIntervalTime += timeOfInterval;
		}
	}
}