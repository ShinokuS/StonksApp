#include "OrderBook.h"

int randomBetween(int begin, int end)
{
	return begin + rand() % (end - begin);
}

// Прячу сюда громоздкую проверку наличия ключа в std::map
#define noBidsWithSuch(price) testOrderBook->bidsAmountForPrice.find(price) == testOrderBook->bidsAmountForPrice.end()
#define noAsksWithSuch(price) testOrderBook->asksAmountForPrice.find(price) == testOrderBook->asksAmountForPrice.end()


// Это способ получить небольшой ордербук без помощи парсера
// и использовать его для тестов UI например.

// В хэдере описан seed по умолчанию, он будет time(0).
// То есть если не пихать seed, то каждый раз будет рандомный ордербук.
// А если пихать константу, то при каждом запуске теста будет одинаковый ордербук.

OrderBook* OrderBook::getTestOrderBook(unsigned int seed)
{
	srand(seed);
	int nAsks = randomBetween(20, 100);
	int nBids = randomBetween(20, 100);
	long long minBidPrice = 100;
	long long minAskPrice = 10000;
	long long maxAskPrice = 20000;
	int maxAmountInOrder = 10000;

	auto testOrderBook = new OrderBook();
	testOrderBook->instrument = "test_instrument_name";

	for (int i = 0; i < nBids; i++) {
		long long price = randomBetween(minBidPrice, minAskPrice);
		if (noBidsWithSuch(price)) {
			testOrderBook->bidsAmountForPrice[price] = 0;
		}
		testOrderBook->bidsAmountForPrice[price] += randomBetween(1, maxAmountInOrder);
	}

	for (int i = 0; i < nAsks; i++) {
		long long price = randomBetween(minAskPrice, maxAskPrice);
		if (noAsksWithSuch(price)) {
			testOrderBook->asksAmountForPrice[price] = 0;
		}
		testOrderBook->asksAmountForPrice[price] += randomBetween(1, maxAmountInOrder);
	}

	// Из std::map очень неудобно доставать первый/последний ключи...
	testOrderBook->minPrice = (*testOrderBook->bidsAmountForPrice.begin()).first;
	testOrderBook->minAskPrice = (*testOrderBook->asksAmountForPrice.begin()).first;
	auto endIter = testOrderBook->bidsAmountForPrice.end();
	testOrderBook->maxPrice = (*(--endIter)).first;

	return testOrderBook;
}