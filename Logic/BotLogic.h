#pragma once

#include "Deals.h"

class BotLogic
{
public:
	//int BORDER_BUY_PRICE = 85;
	//int BORDER_SELL_PRICE = 120;

	int CHEKING_TIME_INTERVAL = 450;
	double PURCHASE_PERCENTAGE = 1.5;
	double SALE_PERCENTAGE = 1.5; 

	int firtsIntervalTime;
	double currentAverPrice;
	double prevAverPrice;

	double sumOfPrices;
	int countOfDeals;
	double AverPrice;

	double sumOfPricesOnInterval;
	int countOfDealsOnInterval;
	int countOfIntervals=1;

	double lastPurchasePrice;

	double botBalance = 1000;
	int botThingsQuantity = 0;

	QVector <double> timeBuy;
	QVector <double> timeSell;

	QVector <double> priceBuy;
	QVector <double> priceSell;

	void reactAtNewDeal(Order* newDeal);
};