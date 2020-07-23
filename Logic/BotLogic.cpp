#include "BotLogic.h"

void BotLogic::reactAtNewDeal(Order* deal)
{
	if (deal->price < BORDER_BUY_PRICE) {
		priceBuy.append(deal->price);
		timeBuy.append(deal->time);
	}
	if (deal->price > BORDER_SELL_PRICE) {
		priceSell.append(deal->price);
		timeSell.append(deal->time);
	}
}