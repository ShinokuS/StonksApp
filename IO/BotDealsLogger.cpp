#include "BotDealsLogger.h"

#include <fstream>

void BotDealsLogger::log(Order* deal, std::string accountName)
{
	std::ofstream logFile(logFileName, std::ios_base::app);

	logFile << accountName << " saw some deal.\n";

	logFile.close();
}