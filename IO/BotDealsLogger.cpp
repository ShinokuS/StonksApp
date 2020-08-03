#include "BotDealsLogger.h"

#include <fstream>

#include <QDate>

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
	QDateTime dateTime;
	dateTime.setTime_t(deal.time);
	std::string date = dateTime.date().toString("dd.MM.yyyy").toStdString();
	std::string time = dateTime.time().toString("HH:mm:ss.zzz").toStdString();

	// Имя инструмента пока что захардкодил. Надо будет его протащить из мэйна сюда.
	std::ofstream logFile(logFileName, std::ios_base::app);
	logFile << tradeId << ";;;" << accountName << ";;" << "ETH" << ";;;" <<
		date << ";" << time << ";;;" << deal.quantity << ";" << deal.price << ";;\n";
	tradeId++;
}