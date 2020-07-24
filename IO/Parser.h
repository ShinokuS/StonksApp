#pragma once

#include <string>
#include <vector>

#include "../Logic/OrderBook.h"

class Parser
{
public:
	void openFile(std::string fileName);
	void setInstrumentName(std::string nameToSet);
	void setDealsStorage(std::vector<Order*>* newDealsStorage);

	OrderBook* parsePreDayOrders();
	void parseDaytimeStuff();

private:
	static const size_t START_POS = 0;
	size_t placeWherePreDayEnded;

	FILE* dumpFile;
	size_t filesize;

	std::string instrumentName;

	std::vector<Order*>* ordersStorage;
	std::vector<Order*>* dealsStorage;

	std::string* readOrdersJsonFromPoint(size_t& i);
	std::string* readDealsJsonFromPoint(size_t& i);
};
