#include <string>
#include <vector>

#include "../Logic/OrderBook.h"

class Parser
{
public:
	 static OrderBook* parsePreDayOrders(std::string fileName, std::string instrumentName);
	 static OrderBook* ParseDaytimeOrders(std::string fileName, std::string instrumentName, OrderBook* orderBook);
	 static std::vector<Order*>* Parser::ParseDaytimeDeal(std::string fileName, std::string instrumentName);
};


