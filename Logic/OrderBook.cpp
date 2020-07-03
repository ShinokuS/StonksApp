#include "OrderBook.h"

#include <chrono>

int randomBetween(int begin, int end)
{
	return begin + rand() % (end - begin);
}

// Это способ получить небольшой ордербук без помощи парсера
// и использовать его для тестов UI например.

// Если не пихать seed, то каждый раз будет рандомный ордербук.
// Если пихать константу, то при каждом запуске теста будет одинаковый ордербук.

OrderBook* OrderBook::getTestOrderBook(unsigned int seed = time(0))
{
	srand(seed);
	int nAsks = randomBetween(1, 10);
	int nBids = randomBetween(1, 10);
	long long minBidPrice = 100;
	long long minAskPrice = 10000;
	long long maxAskPrice = 20000;
	int maxAmountInOrder = 10000;

	auto testOrderBook = new OrderBook();
	testOrderBook->instrument = "test_instrument_name";

	for (int i = 0; i < nBids; i++) {
		long long price = randomBetween(minBidPrice, minAskPrice);
		
		// В с++ это значит "если такого ключа в map ещё нет"
		if (testOrderBook->bidsAmountForPrice.find(price) ==
								testOrderBook->bidsAmountForPrice.end()) {
			testOrderBook->bidsAmountForPrice[price] = 0;
		}

		testOrderBook->bidsAmountForPrice[price] += randomBetween(1, maxAmountInOrder);
	}

	for (int i = 0; i < nAsks; i++) {
		long long price = randomBetween(minAskPrice, maxAskPrice);

		// В с++ это значит "если такого ключа в map ещё нет"
		if (testOrderBook->asksAmountForPrice.find(price) ==
								testOrderBook->asksAmountForPrice.end()) {
			testOrderBook->asksAmountForPrice[price] = 0;
		}

		testOrderBook->asksAmountForPrice[price] += randomBetween(1, maxAmountInOrder);
	}

	// Ёбаный с++ не может красиво доставать первый/последний ключи из map.
	// Упаковывать в функции лень.
	testOrderBook->minPrice = (*testOrderBook->bidsAmountForPrice.begin()).first;
	testOrderBook->minAskPrice = (*testOrderBook->asksAmountForPrice.begin()).first;
	auto endIter = testOrderBook->bidsAmountForPrice.end();
	testOrderBook->maxPrice = (*(--endIter)).first;

	return testOrderBook;
}