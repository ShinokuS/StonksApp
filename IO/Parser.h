#include "../Logic/OrderBookTableModel.h"
#include "string"

class Parser
{
public:
	 static OrderBookTableModel* Parse(std::string fileName, std::string instrumentName);
	 static OrderBookTableModel* Parse(std::string fileName, std::string instrumentName, OrderBookTableModel* orderBookTable);
};


