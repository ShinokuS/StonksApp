#include <string>
#include <vector>

#include "../Logic/OrderBook.h"

class Parser
{
public:
	static void setDealsStorage(std::vector<Order*>* newDealsStorage);
	static OrderBook* parsePreDayOrders(std::string fileName, std::string instrumentName);
	static OrderBook* ParseDaytimeOrders(std::string fileName, std::string instrumentName, OrderBook* orderBook);
	static void Parser::ParseDaytimeDeal(std::string fileName, std::string instrumentName);
};


