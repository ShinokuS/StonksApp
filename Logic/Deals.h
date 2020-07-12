#pragma once

#include "Order.h"

#include <QtGlobal>

class Deals
{
public:
	void addNewDeal(qreal price, qreal quantity, time_t time);
	void addDealForLineGraph();

private:
	QList<Order*> deals;
	QList<Order*> dealsForLineGraph;
	int firstTimeOfInterval;
};