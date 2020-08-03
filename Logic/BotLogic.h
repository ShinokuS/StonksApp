#pragma once

#include "Deals.h"
#include "../IO/BotDealsLogger.h"

class BotLogic
{
public:

	void reactAtNewDeal(Order* newDeal);

	void buyAfter(Order* deal);
	void sellAfter(Order* deal);

	const double START_BALANCE = 1000;
	double botBalance = START_BALANCE;
	int botThingsQuantity = 0;

	QVector <double> timeBuy;
	QVector <double> timeSell;

	QVector <double> priceBuy;
	QVector <double> priceSell;

private:

	std::string accountName = "basic_bot";
	BotDealsLogger dealsLogger = BotDealsLogger();

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