﻿#pragma once

#include <string>
#include <vector>

#include "rapidjson/document.h"

#include "../Logic/OrderBook.h"

class Parser
{
public:
	void openFile(std::string fileName);
	void setInstrumentName(std::string nameToSet);
	void setDealsStorage(std::vector<Order>* newDealsStorage);

	OrderBook* parsePreDayOrders();
	void parseDaytimeStuff();

	bool hasFinished = false;

private:
	static const size_t START_POS = 0;
	size_t i;

	FILE* dumpFile;
	size_t filesize;

	std::string instrumentName;

	std::vector<Order>* ordersStorage;
	std::vector<Order>* dealsStorage;

	std::string* readOrdersJsonFromHere();
	std::string* readDealsJsonFromHere();

	void parseOrdersFromDocument(rapidjson::Document* doc);
	void parseDealsFromDocument(rapidjson::Document* doc);
};
