#pragma once

#include "Order.h"

#include <QtGlobal>

class Deals
{
public:
	QList<Order*> dealsForLineGraph;

	void addNewDeal(qreal price, qreal quantity, time_t time);

private:
	int firstIntervalTime;
	int timeOfInterval = 5;
};