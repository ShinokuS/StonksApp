#pragma once

#include "Order.h"

#include <QtGlobal>

class Deals
{
public:
	void addNewDeal(qreal price, qreal quantity, time_t time);
	void addDealForLineGraph(Order* newDeal);

private:
	int firstIntervalTime;

	QList<Order*> dealsForLineGraph;
};