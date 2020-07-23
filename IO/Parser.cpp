#define _CRT_SECURE_NO_WARNINGS

#include <string>

#include "rapidjson/document.h"

#include "../Logic/OrderBook.h"
#include "Parser.h"

//Это для файлового указателя, чтобы знать, где уже прочитан файл, а где нет.
size_t ordersPlace = 0;
size_t dealsPlace = 0;

time_t times;
const int TIME_SPACE = 600000;

//Метод для парса в новую таблицу ордеров
OrderBook* Parser::parsePreDayOrders(std::string fileName, std::string instrumentName)
{
	FILE* dumpFile = fopen(fileName.c_str(), "rb");

	_fseeki64(dumpFile, 0, SEEK_END);
	size_t filesize = (size_t)_ftelli64(dumpFile);//определяем размер файла.
	_fseeki64(dumpFile, ordersPlace, SEEK_SET);
	
	std::string search;//Буффер для поиска ключевых слов
	const std::string keyWord = "book."+instrumentName; 
	search.resize(keyWord.size());

	OrderBook* orderBookTable = new OrderBook; //Книжка для заполнения ордерами

	for (size_t i = ordersPlace; i < filesize; ++i)//Начинаем поиск по файлу
	{
		for (int j = 0; j < search.size()-1; ++j)
		{
			search[j] = search[j+1];
		}
		search.back() = fgetc(dumpFile);

		if (search == (keyWord))	//Если находим ключевое слово, начинаем считывать чистую json-строку
		{
			std::string json = "{";
			while (fgetc(dumpFile) != '{');
			json.reserve(100000);
			char ch;
			short rightCurlyBrackets = 0;
			short leftCurlyBrackets = 1;
			while (leftCurlyBrackets != rightCurlyBrackets)
			{
				ch = fgetc(dumpFile);//Считываем посимвольно json-строку для Document.
				++i;
				json += ch; //Формируем строку для Document.
				if (ch == '{')
				{
					++leftCurlyBrackets;
				}
				else if (ch == '}')
				{
					++rightCurlyBrackets;
				}
			}
			rapidjson::Document* doc = new rapidjson::Document;
			doc->Parse(json.c_str());
			time_t timestamp = (*doc)["timestamp"].GetInt64();

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

			times = timestamp;
			delete doc;
			break;
		}
	}
	ordersPlace = (size_t)_ftelli64(dumpFile);
	return orderBookTable;
}
//Второй метод для парса в уже существующуу таблицу
OrderBook* Parser::ParseDaytimeOrders(std::string fileName, std::string instrumentName, OrderBook* orderBook) 
{
	FILE* dumpFile = fopen(fileName.c_str(), "rb");

	_fseeki64(dumpFile, 0, SEEK_END);
	size_t filesize = (size_t)_ftelli64(dumpFile);//определяем размер файла.
	_fseeki64(dumpFile, ordersPlace, SEEK_SET);

	std::string search;//Буффер для поиска ключевых слов
	const std::string keyWord = "book." + instrumentName;
	search.resize(keyWord.size());


	for (size_t i = ordersPlace; i < filesize; ++i)//Начинаем поиск по файлу
	{
		for (int j = 0; j < search.size() - 1; ++j)
		{
			search[j] = search[j + 1];
		}
		search.back() = fgetc(dumpFile);
		if (search == (keyWord))	//Если находим ключевое слово, начинаем считывать чистую json-строку
		{
			std::string json = "{";
			while (fgetc(dumpFile) != '{');
			json.reserve(100000);
			char ch;
			short rightCurlyBrackets = 0;
			short leftCurlyBrackets = 1;
			while (leftCurlyBrackets != rightCurlyBrackets)
			{
				ch = fgetc(dumpFile);//Считываем посимвольно json-строку для Document.
				++i;
				json += ch; //Формируем строку для Document.
				if (ch == '{')
				{
					++leftCurlyBrackets;
				}
				else if (ch == '}')
				{
					++rightCurlyBrackets;
				}
			}
			rapidjson::Document* doc = new rapidjson::Document;
			doc->Parse(json.c_str());
			time_t timestamp = (*doc)["timestamp"].GetInt64();

			for (auto itr = (*doc)["bids"].Begin(); itr != (*doc)["bids"].End(); ++itr) //Прогоняемся по массиву bids для заполнения книжки
			{
				std::string flag = (*itr)[0].GetString();
				qreal price = (*itr)[1].GetDouble();
				qreal quantity = (*itr)[2].GetDouble();
				auto newBid = new Order{ price, quantity, false, timestamp, flag };
				orderBook->addOrder(newBid);
			}

			for (auto itr = (*doc)["asks"].Begin(); itr != (*doc)["asks"].End(); ++itr)	//Прогоняемся по массиву asks для заполнения книжки
			{
				std::string flag = (*itr)[0].GetString();
				qreal price = (*itr)[1].GetDouble();
				qreal quantity = (*itr)[2].GetDouble();
				auto newAsk = new Order{ price, quantity, true, timestamp, flag };
				orderBook->addOrder(newAsk);
			}
			delete doc;
			if (times + TIME_SPACE < timestamp)
			{
				times = timestamp;
				break;
			}

		}
	}

	ordersPlace = (size_t)_ftelli64(dumpFile);
	return orderBook;
}

std::vector<Order*>* Parser::ParseDaytimeDeal(std::string fileName, std::string instrumentName)
{
	auto result = new std::vector<Order*>();
	result->reserve(1000000);

	FILE* dumpFile = fopen(fileName.c_str(), "rb");

	_fseeki64(dumpFile, 0, SEEK_END);
	size_t filesize = (size_t)_ftelli64(dumpFile);//определяем размер файла.
	_fseeki64(dumpFile, dealsPlace, SEEK_SET);

	std::string search;//Буффер для поиска ключевых слов
	const std::string keyWord = "trades." + instrumentName;
	search.resize(keyWord.size());


	for (size_t i = dealsPlace; i < filesize; ++i)//Начинаем поиск по файлу
	{
		for (int j = 0; j < search.size() - 1; ++j)
		{
			search[j] = search[j + 1];
		}
		search.back() = fgetc(dumpFile);
		if (search == (keyWord))	//Если находим ключевое слово, начинаем считывать чистую json-строку
		{
			std::string json = "{\"data\":[";
			while (fgetc(dumpFile) != '[');
			json.reserve(100000);
			char ch;
			short rightCurlyBrackets = 0;
			short leftCurlyBrackets = 1;
			while (leftCurlyBrackets != rightCurlyBrackets)
			{
				ch = fgetc(dumpFile);//Считываем посимвольно json-строку для Document.
				++i;
				json += ch; //Формируем строку для Document.
				if (ch == '[')
				{
					++leftCurlyBrackets;
				}
				else if (ch == ']')
				{
					++rightCurlyBrackets;
				}
			}
			json += "}";
			rapidjson::Document* doc = new rapidjson::Document;
			doc->Parse(json.c_str());
			for (auto it = (*doc)["data"].Begin(); it != (*doc)["data"].End(); ++it)
			{
				time_t time = (*it)["timestamp"].GetInt64();
				qreal price = (*it)["price"].GetDouble();
				qreal quantity = (*it)["amount"].GetDouble();
				auto newDeal = new Order{ price, quantity, false, time };
				result->push_back(newDeal);
			}
			delete doc;
			break;
		}
	}
	dealsPlace = (size_t)_ftelli64(dumpFile);
	return result;
}