#include <string>
#include <vector>

#include "../Logic/OrderBook.h"

class Parser
{
public:
	static void openFile(std::string fileName);
	static void setInstrumentName(std::string nameToSet);
	static void setDealsStorage(std::vector<Order*>* newDealsStorage);
	static OrderBook* parsePreDayOrders();
	static OrderBook* ParseDaytimeOrders(OrderBook* orderBook);
	static void Parser::ParseDaytimeDeal();
private:
	static std::string* readOrdersJsonFromPoint(size_t& i);
	static std::string* readDealsJsonFromPoint(size_t& i);
};


