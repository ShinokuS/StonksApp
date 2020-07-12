#pragma once

#include "Order.h"

#include <QtGlobal>

class Deals
{
public:
	void addNewDeal(qreal price, time_t time);
	void getLastDeal(qreal price, time_t time);

private:
	Order* lastDeal;
};