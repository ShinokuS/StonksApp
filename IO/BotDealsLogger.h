﻿#pragma once

#include "../Logic/Order.h"

class BotDealsLogger
{
public:

	std::string logFileName = "bot_deals.csv";

	BotDealsLogger();
	void log(Order deal, std::string accountName);

private:

	long long tradeId = 0;
};