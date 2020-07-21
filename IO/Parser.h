#include <string>

#include "../Logic/OrderBookTableModel.h"

class Parser
{
public:
	 static OrderBookTableModel* parsePreDayOrders(std::string fileName, std::string instrumentName);
	 static OrderBookTableModel* ParseDaytimeOrders(std::string fileName, std::string instrumentName, OrderBookTableModel* orderBookTable);
};


