#include <string>

#include "../Logic/OrderBookTableModel.h"

class Parser
{
public:
	 static OrderBookTableModel* Parse(std::string fileName, std::string instrumentName);
	 static OrderBookTableModel* Parse(std::string fileName, std::string instrumentName, OrderBookTableModel* orderBookTable);
};


