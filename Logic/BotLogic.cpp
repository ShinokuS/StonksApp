#include "BotLogic.h"

void BotLogic::reactAtNewDeal(Order* deal)
{
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
		100 - currentAverPrice * 100 / prevAverPrice > PURCHASE_PERCENTAGE && currentAverPrice<AverPrice) {
		
		buyAfter(deal);
	}
	if (botThingsQuantity && 
		100 - lastPurchasePrice * 100 / deal->price > SALE_PERCENTAGE) {
		
		sellAfter(deal);
	}

}

void BotLogic::buyAfter(Order* deal)
{
	int amountToBuy = int(botBalance / deal->price);
	botThingsQuantity += amountToBuy;
	botBalance -= amountToBuy * deal->price;
	if (!lastPurchasePrice) lastPurchasePrice = deal->price;

	priceBuy.append(deal->price);
	timeBuy.append(deal->time);
}

void BotLogic::sellAfter(Order* deal)
{
	botBalance += botThingsQuantity * deal->price;
	botThingsQuantity = 0;
	lastPurchasePrice = 0;

	priceSell.append(deal->price);
	timeSell.append(deal->time);
}