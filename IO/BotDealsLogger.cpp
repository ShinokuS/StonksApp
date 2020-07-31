#include "BotDealsLogger.h"

#include <fstream>

void BotDealsLogger::log(Order deal, std::string accountName)
{
	std::ofstream logFile(logFileName, std::ios_base::app);

	logFile << accountName << " bought or sold.\n";

	logFile.close();
}