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
	std::string* readOrdersJsonFromPoint(size_t& i);
	std::string* readDealsJsonFromPoint(size_t& i);
};
