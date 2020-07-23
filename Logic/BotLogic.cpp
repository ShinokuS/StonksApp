#include "BotLogic.h"

void BotLogic::reactAtNewDeal(Order* deal)
{
	/*
	if (deal->price < BORDER_BUY_PRICE) {
		priceBuy.append(deal->price);
		timeBuy.append(deal->time);
	}
	if (deal->price > BORDER_SELL_PRICE) {
		priceSell.append(deal->price);
		timeSell.append(deal->time);
	}
	*/
	sumOfPrices += deal->price;
	countOfDeals++;
	AverPrice = sumOfPrices / countOfDeals;
	sumOfPricesOnInterval += deal->price;
	countOfDealsOnInterval++;

	if (!firtsIntervalTime) firtsIntervalTime = deal->time;
	if (deal->time - firtsIntervalTime > CHEKING_TIME_INTERVAL) {
		firtsIntervalTime += CHEKING_TIME_INTERVAL;
		countOfIntervals++;
		prevAverPrice = currentAverPrice;
		currentAverPrice = sumOfPricesOnInterval / countOfDealsOnInterval; 
		sumOfPricesOnInterval = 0;
		countOfDealsOnInterval = 0;
	}
	if (countOfIntervals % 2 == 0 && botBalance >= deal->price && 
		int(100 - currentAverPrice * 100 / prevAverPrice) > PURCHASE_PERCENTAGE && currentAverPrice<AverPrice) {
		botThingsQuantity += int(botBalance / deal->price);
		botBalance -= int(botBalance / deal->price) * deal->price;
		lastPurchasePrice = deal->price;

		priceBuy.append(deal->price);
		timeBuy.append(deal->time);
	}
	if (botThingsQuantity && 
		int(100 - AverPrice * 100 / deal->price) > SALE_PERCENTAGE) {
		botBalance += botThingsQuantity * deal->price;
		botThingsQuantity = 0;

		priceSell.append(deal->price);
		timeSell.append(deal->time);
	}

}