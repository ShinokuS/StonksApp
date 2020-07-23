#pragma once

#include "Deals.h"

class BotLogic
{
public:
	int BORDER_BUY_PRICE = 85;
	int BORDER_SELL_PRICE = 120;

	QVector <double> timeBuy;
	QVector <double> timeSell;

	QVector <double> priceBuy;
	QVector <double> priceSell;

	BotLogic(QList<Order*> deals);
};