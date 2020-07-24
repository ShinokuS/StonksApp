#define _CRT_SECURE_NO_WARNINGS

#include <string>

#include "rapidjson/document.h"

#include "../Logic/OrderBook.h"
#include "Parser.h"

void Parser::openFile(std::string fileName)
{
	dumpFile = fopen(fileName.c_str(), "rb");

	_fseeki64(dumpFile, 0, SEEK_END);
	filesize = (size_t)_ftelli64(dumpFile);//определяем размер файла.
}

void Parser::setInstrumentName(std::string nameToSet)
{
	instrumentName = nameToSet;
}

void Parser::setDealsStorage(std::vector<Order*>* newDealsStorage)
{
	dealsStorage = newDealsStorage;
}

//Метод для парса в новую таблицу ордеров
OrderBook* Parser::parsePreDayOrders()
{
	_fseeki64(dumpFile, START_POS, SEEK_SET);

	std::string search;//Буффер для поиска ключевых слов
	const std::string keyWord = "book." + instrumentName;
	search.resize(keyWord.size());

	ordersStorage = new std::vector<Order*>;
	ordersStorage->reserve(5000000);
	OrderBook* orderBookTable = new OrderBook(ordersStorage); //Книжка для заполнения ордерами

	for (size_t i = START_POS; i < filesize; ++i)//Начинаем поиск по файлу
	{
		for (int j = 0; j < search.size() - 1; ++j)
		{
			search[j] = search[j + 1];
		}
		search.back() = fgetc(dumpFile);

		if (search == (keyWord))	//Если находим ключевое слово, начинаем считывать чистую json-строку
		{
			auto json = readOrdersJsonFromPoint(i);
			rapidjson::Document* doc = new rapidjson::Document;
			doc->Parse(json->c_str());

			// деление это отбрасывание долей секунд для конвертации в юникстайм
			time_t timestamp = (*doc)["timestamp"].GetInt64() / 10000;

			for (auto itr = (*doc)["bids"].Begin(); itr != (*doc)["bids"].End(); ++itr) //Прогоняемся по массиву bids для заполнения книжки
			{
				std::string flag = (*itr)[0].GetString();
				qreal price = (*itr)[1].GetDouble();
				qreal quantity = (*itr)[2].GetDouble();
				auto newBid = new Order{ price, quantity, false, timestamp, flag };
				orderBookTable->addOrder(newBid);
			}

			for (auto itr = (*doc)["asks"].Begin(); itr != (*doc)["asks"].End(); ++itr)	//Прогоняемся по массиву asks для заполнения книжки
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
	placeWherePreDayEnded = (size_t)_ftelli64(dumpFile);
	return orderBookTable;
}


void Parser::parseDaytimeOrders()
{
	_fseeki64(dumpFile, placeWherePreDayEnded, SEEK_SET);

	std::string ordersSearchBuffer;//Буффер для поиска ключевых слов
	const std::string ordersJsonTitle = "book." + instrumentName;
	ordersSearchBuffer.resize(ordersJsonTitle.size());


	for (size_t i = placeWherePreDayEnded; i < filesize; ++i)//Начинаем поиск по файлу
	{
		for (int j = 0; j < ordersSearchBuffer.size() - 1; ++j)
		{
			ordersSearchBuffer[j] = ordersSearchBuffer[j + 1];
		}
		ordersSearchBuffer.back() = fgetc(dumpFile);
		if (ordersSearchBuffer == (ordersJsonTitle))	//Если находим ключевое слово, начинаем считывать чистую json-строку
		{
			auto json = readOrdersJsonFromPoint(i);
			rapidjson::Document* doc = new rapidjson::Document;
			doc->Parse(json->c_str());

			// деление это отбрасывание долей секунд для конвертации в юникстайм
			time_t timestamp = (*doc)["timestamp"].GetInt64() / 10000;

			for (auto itr = (*doc)["bids"].Begin(); itr != (*doc)["bids"].End(); ++itr) //Прогоняемся по массиву bids для заполнения книжки
			{
				std::string flag = (*itr)[0].GetString();
				qreal price = (*itr)[1].GetDouble();
				qreal quantity = (*itr)[2].GetDouble();
				auto newBid = new Order{ price, quantity, false, timestamp, flag };
				ordersStorage->push_back(newBid);
			}

			for (auto itr = (*doc)["asks"].Begin(); itr != (*doc)["asks"].End(); ++itr)	//Прогоняемся по массиву asks для заполнения книжки
			{
				std::string flag = (*itr)[0].GetString();
				qreal price = (*itr)[1].GetDouble();
				qreal quantity = (*itr)[2].GetDouble();
				auto newAsk = new Order{ price, quantity, true, timestamp, flag };
				ordersStorage->push_back(newAsk);
			}

			delete doc;
			delete json;
		}
	}
}

void Parser::parseDeals()
{
	_fseeki64(dumpFile, placeWherePreDayEnded, SEEK_SET);

	std::string dealsSearchBuffer;//Буффер для поиска ключевых слов
	const std::string dealsJsonTitle = "trades." + instrumentName;
	dealsSearchBuffer.resize(dealsJsonTitle.size());


	for (size_t i = placeWherePreDayEnded; i < filesize; ++i)//Начинаем поиск по файлу
	{
		for (int j = 0; j < dealsSearchBuffer.size() - 1; ++j)
		{
			dealsSearchBuffer[j] = dealsSearchBuffer[j + 1];
		}
		dealsSearchBuffer.back() = fgetc(dumpFile);
		if (dealsSearchBuffer == (dealsJsonTitle))	//Если находим ключевое слово, начинаем считывать чистую json-строку
		{
			auto json = readDealsJsonFromPoint(i);
			rapidjson::Document* doc = new rapidjson::Document;
			doc->Parse(json->c_str());

			for (auto it = (*doc)["data"].Begin(); it != (*doc)["data"].End(); ++it)
			{
				// деление это отбрасывание долей секунд для конвертации в юникстайм
				time_t time = (*it)["timestamp"].GetInt64() / 10000;
				qreal price = (*it)["price"].GetDouble();
				qreal quantity = (*it)["amount"].GetDouble();
				auto newDeal = new Order{ price, quantity, false, time };
				dealsStorage->push_back(newDeal);
			}

			delete doc;
			delete json;
		}
	}
}

std::string* Parser::readOrdersJsonFromPoint(size_t& i)
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

std::string* Parser::readDealsJsonFromPoint(size_t& i)
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