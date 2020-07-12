#pragma once

#include "Order.h"

#include <QtGlobal>

class Deals
{
public:
	QList<Order*> dealsForLineGraph;

	void addNewDeal(qreal price, qreal quantity, time_t time);
	void addDealForLineGraph(Order* newDeal);

private:
	int firstIntervalTime;
};