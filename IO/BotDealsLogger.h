#pragma once

#include <QString>

#include "../Logic/Order.h"

class BotDealsLogger
{
	void log(Order* deal, QString accountName);
};