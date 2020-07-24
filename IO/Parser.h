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
	void ParseDaytimeOrders();
	void ParseDaytimeDeal();

private:
	//Это для файлового указателя, чтобы знать, где уже прочитан файл, а где нет.
	size_t ordersPlace = 0;
	size_t dealsPlace = 0;

	FILE* dumpFile;
	size_t filesize;

	std::string instrumentName;

	std::vector<Order*>* ordersStorage;
	std::vector<Order*>* dealsStorage;

	std::string* readOrdersJsonFromPoint(size_t& i);
	std::string* readDealsJsonFromPoint(size_t& i);
};
