#include "Logic/OrderBookTableModel.h"
#include "string"

class Parser
{
public:
	 static OrderBookTableModel* Parse(std::string fileName, std::string instrumentName);
};


