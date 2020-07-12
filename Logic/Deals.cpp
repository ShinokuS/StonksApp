#include "Deals.h"

void Deals::addNewDeal(qreal price, time_t time)
{
	lastDeal = new Order{ price, 0, true, time };
}

Order* Deals::getLastDeal()
{
	return lastDeal;
}