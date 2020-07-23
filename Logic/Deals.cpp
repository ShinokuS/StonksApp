#include "Deals.h"

Deals::Deals(std::vector<Order*>* dealsSource)
{
	this->dealsSource = dealsSource;
	activeDealIndexInSource = -1;
}

bool Deals::canLoadNextDealFromSource()
{
	return activeDealIndexInSource < dealsSource->size() - 1;
}

void Deals::loadNextDealFromSource()
{
	activeDealIndexInSource++;
	//addNewDeal((*dealsSource)[activeDealIndexInSource]);
	dealsForPriceGraph.append((*dealsSource)[activeDealIndexInSource]);
}

Order* Deals::getLastDeal()
{
	return dealsForPriceGraph.back();
}

void Deals::addNewDeal(Order* deal)
{
	if (minPrice > deal->price || minPrice==0) {
		minPrice = deal->price;
	}
	if (maxPrice < deal->price) {
		maxPrice = deal->price;
	}
	if (!firstIntervalTime) {
		dealsForPriceGraph.append(deal);
		firstIntervalTime = deal->time;
	}
	else {
		if (deal->time - firstIntervalTime < timeOfInterval) {
			delete dealsForPriceGraph.last();
			dealsForPriceGraph.last() = deal;
		}
		else {
			dealsForPriceGraph.append(deal);
			firstIntervalTime += timeOfInterval;
		}
	}
}

qreal randomBetween(int begin, int end)
{
	return qreal(begin + rand() % (end - begin));
}

std::vector<Order*>* Deals::makeRandomListOfDeals(unsigned int seed)
{
	auto randomDeals = new std::vector<Order*>();
	randomDeals->reserve(Time::DAY / 300);

	srand(seed);

	int firstDayTime = (time(0) / Time::DAY) * Time::DAY;
	qreal price = randomBetween(100, 150), cicle;
	bool upOrDown = randomBetween(0, 2);

	for (int i = firstDayTime; i != firstDayTime + Time::DAY; i += 300) {
		if (upOrDown) {
			price += randomBetween(0, 5);
		}
		else {
			price -= randomBetween(0, 5);
		}
		cicle += 1;
		if (cicle == 10) {
			cicle = 0;
			if (price < 30) {
				upOrDown = true;
			}
			else upOrDown = randomBetween(0, 2);
		}
		auto newDeal = new Order{ price, randomBetween(100, 5000), 0, i };

		randomDeals->push_back(newDeal);
	}

	return randomDeals;
}