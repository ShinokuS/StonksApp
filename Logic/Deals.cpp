#include "Deals.h"

void Deals::addNewDeal(qreal price, qreal quantity, time_t time)
{
	auto newDeal = new Order{ price, quantity, 0, time };

	if (minPrice > price || minPrice==0) {
		minPrice = price;
	}
	if (maxPrice < price) {
		maxPrice = price;
	}
	if (!firstIntervalTime) {
		dealsForPriceGraph.append(newDeal);
		firstIntervalTime = newDeal->time;
	}
	else {
		if (newDeal->time - firstIntervalTime < timeOfInterval) {
			delete dealsForPriceGraph.last();
			dealsForPriceGraph.last() = newDeal;
		}
		else {
			dealsForPriceGraph.append(newDeal);
			firstIntervalTime += timeOfInterval;
		}
	}
}

qreal randomBetween(int begin, int end)
{
	return qreal(begin + rand() % (end - begin));
}

void Deals::randomListOfDeals(unsigned int seed)
{
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

		dealsForPriceGraph.append(newDeal);
	}

}