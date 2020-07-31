#pragma once

#include "../Logic/Order.h"

class BotDealsLogger
{
public:

	std::string logFileName = "bot_deals.csv";

	void log(Order deal, std::string accountName);

};