#include "BotDealsLogger.h"

#include <fstream>

bool fileIsEmptyOrDoesntExist(const std::string& fileName)
{
	std::ifstream file(fileName);
	return file.peek() == std::ifstream::traits_type::eof();
}

BotDealsLogger::BotDealsLogger()
{
	// Заботимся о заголовках
	if (fileIsEmptyOrDoesntExist(logFileName)) {
		std::ofstream logFile(logFileName, std::ios_base::trunc);
		logFile << "SN;Flags;WinId;Account;Connector;Ticker;AuxCode;SecId;Date;"
			<< "Time;TradeId;OrderId;Quantity;Price;Result\n";
	}
}

void BotDealsLogger::log(Order deal, std::string accountName)
{
	std::ofstream logFile(logFileName, std::ios_base::app);
	logFile << accountName << " bought or sold.\n";
}