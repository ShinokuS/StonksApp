#include <string>

#include "../Logic/OrderBook.h"

class Parser
{
public:
	 static OrderBook* parsePreDayOrders(std::string fileName, std::string instrumentName);
	 static OrderBook* ParseDaytimeOrders(std::string fileName, std::string instrumentName, OrderBook* orderBook);
};


