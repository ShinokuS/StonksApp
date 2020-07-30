#pragma once

#include "Deals.h"

class BotLogic
{
public:

	void reactAtNewDeal(Order* newDeal);

	double botBalance = 1000;
	int botThingsQuantity = 0;

	QVector <double> timeBuy;
	QVector <double> timeSell;

	QVector <double> priceBuy;
	QVector <double> priceSell;

private:

	int CHEKING_TIME_INTERVAL = 30;
	double PURCHASE_PERCENTAGE = 0.01;
	double SALE_PERCENTAGE = 0.1;

	int firtsIntervalTime;
	double currentAverPrice;
	double prevAverPrice;

	double sumOfPrices;
	int countOfDeals;
	double AverPrice;

	double sumOfPricesOnInterval;
	int countOfDealsOnInterval;
	int countOfIntervals = 1;

	double lastPurchasePrice;
};