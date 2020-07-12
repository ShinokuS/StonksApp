#pragma once

#include "Order.h"

#include <QtGlobal>

class Deals
{
public:
	void addNewDeal(qreal price, time_t time);
	Order* getLastDeal();

private:
	Order* lastDeal;
};