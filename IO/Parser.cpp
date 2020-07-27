#define _CRT_SECURE_NO_WARNINGS

#include <string>

#include "../Logic/OrderBook.h"
#include "Parser.h"

void Parser::openFile(std::string fileName)
{
	dumpFile = fopen(fileName.c_str(), "rb");

	_fseeki64(dumpFile, 0, SEEK_END);
	filesize = (size_t)_ftelli64(dumpFile);
}

void Parser::setInstrumentName(std::string nameToSet)
{
	instrumentName = nameToSet;
}

void Parser::setDealsStorage(std::vector<Order>* newDealsStorage)
{
	dealsStorage = newDealsStorage;
}

// Создаёт новый ордербук и распарсивает туда ордеры, которые были в первый момент дня.
OrderBook* Parser::parsePreDayOrders()
{
	_fseeki64(dumpFile, START_POS, SEEK_SET);

	// Буфер для поиска заголовка нужного джейсона
	std::string ordersSearchBuffer;
	const std::string ordersJsonBuffer = "book." + instrumentName;
	ordersSearchBuffer.resize(ordersJsonBuffer.size());

	// Создание ордербука, который заполним и вернём
	ordersStorage = new std::vector<Order>;
	ordersStorage->reserve(5000000);
	OrderBook* orderBookTable = new OrderBook(ordersStorage);

	// Листание файла
	for (i = START_POS; i < filesize; ++i)
	{
		for (int j = 0; j < ordersSearchBuffer.size() - 1; ++j)
		{
			ordersSearchBuffer[j] = ordersSearchBuffer[j + 1];
		}
		ordersSearchBuffer.back() = fgetc(dumpFile);

		// Нашли начало джейсона с ордерами
		if (ordersSearchBuffer == (ordersJsonBuffer))
		{
			auto json = readOrdersJsonFromHere();
			rapidjson::Document* doc = new rapidjson::Document;
			doc->Parse(json->c_str());

			// Деление это отбрасывание долей секунд для конвертации в юникстайм
			time_t timestamp = (*doc)["timestamp"].GetInt64() / 10000;

			for (auto itr = (*doc)["bids"].Begin(); itr != (*doc)["bids"].End(); ++itr)
			{
				std::string flag = (*itr)[0].GetString();
				qreal price = (*itr)[1].GetDouble();
				qreal quantity = (*itr)[2].GetDouble();
				auto newBid = new Order{ price, quantity, false, timestamp, flag };
				orderBookTable->addOrder(newBid);
			}

			for (auto itr = (*doc)["asks"].Begin(); itr != (*doc)["asks"].End(); ++itr)
			{
				std::string flag = (*itr)[0].GetString();
				qreal price = (*itr)[1].GetDouble();
				qreal quantity = (*itr)[2].GetDouble();
				auto newAsk = new Order{ price, quantity, true, timestamp, flag };
				orderBookTable->addOrder(newAsk);
			}

			delete doc;
			delete json;
			break;
		}
	}
	i = (size_t)_ftelli64(dumpFile);
	return orderBookTable;
}

void Parser::parseDaytimeStuff()
{
	_fseeki64(dumpFile, i, SEEK_SET);

	// Основной буфер для поиска нужной инфы в файле.
	std::string dealsSearchBuffer;
	const std::string dealsJsonTitle = "trades." + instrumentName;
	dealsSearchBuffer.resize(dealsJsonTitle.size());

	// Этот указатель для заглядывания в буфер начиная с третьего символа,
	// потому что ключевое слово для ордеров короче, чем для сделок.
	const char* ordersSearchBuffer = &dealsSearchBuffer.c_str()[2];

	// Написать одной строкой не работает, надо чтоб std::string отдельно тут лежала.
	const std::string ordersJsonTitle_StdString = ("book." + instrumentName);
	const char* ordersJsonTitle = ordersJsonTitle_StdString.c_str();

	// Листание файла
	for (/*     */; i < filesize; ++i)
	{
		for (int j = 0; j < dealsSearchBuffer.size() - 1; ++j)
		{
			dealsSearchBuffer[j] = dealsSearchBuffer[j + 1];
		}
		dealsSearchBuffer.back() = fgetc(dumpFile);

		// Нашли начало джейсона с ордерами
		if (!strcmp(ordersSearchBuffer, ordersJsonTitle))
		{
			auto json = readOrdersJsonFromHere();
			rapidjson::Document* doc = new rapidjson::Document;
			doc->Parse(json->c_str());
			parseOrdersFromDocument(doc);

			delete doc;
			delete json;
		}
		// Нашли начало джейсона со сделками
		if (dealsSearchBuffer == (dealsJsonTitle))
		{
			auto json = readDealsJsonFromHere();
			rapidjson::Document* doc = new rapidjson::Document;
			doc->Parse(json->c_str());
			parseDealsFromDocument(doc);

			delete doc;
			delete json;
		}
	}
	fclose(dumpFile);
}

std::string* Parser::readOrdersJsonFromHere()
{
	auto json = new std::string("{");
	while (fgetc(dumpFile) != '{'); // Мотаем поток до начала тела джейсона
	json->reserve(100000);
	char ch;
	short rightCurlyBrackets = 0;
	short leftCurlyBrackets = 1;
	while (leftCurlyBrackets != rightCurlyBrackets)
	{
		ch = fgetc(dumpFile); // Считываем по одному символу из файла
		++i;
		*json += ch; // И дописываем их в json-строку
		if (ch == '{')
		{
			++leftCurlyBrackets;
		}
		else if (ch == '}')
		{
			++rightCurlyBrackets;
		}
	}
	return json;
}

std::string* Parser::readDealsJsonFromHere()
{
	auto json = new std::string("{\"data\":[");
	while (fgetc(dumpFile) != '[');  // Мотаем поток до начала тела джейсона
	json->reserve(100000);
	char ch;
	short rightCurlyBrackets = 0;
	short leftCurlyBrackets = 1;
	while (leftCurlyBrackets != rightCurlyBrackets)
	{
		ch = fgetc(dumpFile); // Считываем по одному символу из файла
		++i;
		*json += ch; // И дописываем их в json-строку
		if (ch == '[')
		{
			++leftCurlyBrackets;
		}
		else if (ch == ']')
		{
			++rightCurlyBrackets;
		}
	}
	*json += "}";
	return json;
}

void Parser::parseOrdersFromDocument(rapidjson::Document* doc)
{
	// Деление это отбрасывание долей секунд для конвертации в юникстайм
	time_t timestamp = (*doc)["timestamp"].GetInt64() / 10000;

	for (auto itr = (*doc)["bids"].Begin(); itr != (*doc)["bids"].End(); ++itr)
	{
		std::string flag = (*itr)[0].GetString();
		qreal price = (*itr)[1].GetDouble();
		qreal quantity = (*itr)[2].GetDouble();
		ordersStorage->emplace_back(Order{ price, quantity, false, timestamp, flag });
	}

	for (auto itr = (*doc)["asks"].Begin(); itr != (*doc)["asks"].End(); ++itr)
	{
		std::string flag = (*itr)[0].GetString();
		qreal price = (*itr)[1].GetDouble();
		qreal quantity = (*itr)[2].GetDouble();
		ordersStorage->emplace_back( Order{ price, quantity, true, timestamp, flag });
	}
}

void Parser::parseDealsFromDocument(rapidjson::Document* doc)
{
	for (auto it = (*doc)["data"].Begin(); it != (*doc)["data"].End(); ++it)
	{
		// Деление это отбрасывание долей секунд для конвертации в юникстайм
		time_t time = (*it)["timestamp"].GetInt64() / 10000;
		qreal price = (*it)["price"].GetDouble();
		qreal quantity = (*it)["amount"].GetDouble();
		dealsStorage->emplace_back( Order { price, quantity, false, time });
	}
}