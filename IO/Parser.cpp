#define _CRT_SECURE_NO_WARNINGS
#define time_space 600
#include "string"
#include "IO/rapidjson/document.h"
#include "Logic/OrderBookTableModel.h"
#include "IO/Parser.h"
size_t place = 0; //Это для файлового указателя, чтобы знать, где уже прочитан файл, а где нет.
time_t times;
//Метод для парса в новую таблицу ордеров
OrderBookTableModel* Parser::Parse(std::string fileName, std::string instrumentName)
{
	FILE* dumpFile = fopen(fileName.c_str(), "rb");

	_fseeki64(dumpFile, 0, SEEK_END);
	size_t filesize = (size_t)_ftelli64(dumpFile);//определяем размер файла.
	_fseeki64(dumpFile, place, SEEK_SET);
	
	std::string search;//Буффер для поиска ключевых слов
	const std::string keyWord = "book."+instrumentName; 
	search.resize(keyWord.size());

	OrderBookTableModel* orderBookTable = new OrderBookTableModel; //Книжка для заполнения ордерами

	int counter = 0;

	for (size_t i = place; i < filesize; ++i)//Начинаем поиск по файлу
	{
		for (int j = 0; j < search.size()-1; ++j)
		{
			search[j] = search[j+1];
		}
		search.back() = fgetc(dumpFile);
		if (search == (keyWord))	//Если находим ключевое слово, начинаем считывать чистую json-строку
		{
			if (counter == 10)
			{
				break;
			}
			++counter;

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
				orderBookTable->addBidNew((qreal)(*itr)[1].GetDouble(), (qreal)(*itr)[2].GetDouble(), timestamp, flag);
			} 

			for (auto itr = (*doc)["asks"].Begin(); itr != (*doc)["asks"].End(); ++itr)	//Прогоняемся по массиву asks для заполнения книжки
			{
				std::string flag = (*itr)[0].GetString();
				orderBookTable->addAskNew((qreal)(*itr)[1].GetDouble(), (qreal)(*itr)[2].GetDouble(), timestamp, flag);
			}
			times = timestamp;
			delete doc;
		}
	}
	place = (size_t)_ftelli64(dumpFile);
	return orderBookTable;
}
//Второй метод для парса в уже существующуу таблицу
OrderBookTableModel* Parser::Parse(std::string fileName, std::string instrumentName, OrderBookTableModel* orderBookTable) 
{
	FILE* dumpFile = fopen(fileName.c_str(), "rb");

	_fseeki64(dumpFile, 0, SEEK_END);
	size_t filesize = (size_t)_ftelli64(dumpFile);//определяем размер файла.
	_fseeki64(dumpFile, place, SEEK_SET);

	std::string search;//Буффер для поиска ключевых слов
	const std::string keyWord = "book." + instrumentName;
	search.resize(keyWord.size());


	for (size_t i = place; i < filesize; ++i)//Начинаем поиск по файлу
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
				orderBookTable->addBidNew((qreal)(*itr)[1].GetDouble(), (qreal)(*itr)[2].GetDouble(), timestamp, flag);
			}

			for (auto itr = (*doc)["asks"].Begin(); itr != (*doc)["asks"].End(); ++itr)	//Прогоняемся по массиву asks для заполнения книжки
			{
				std::string flag = (*itr)[0].GetString();
				orderBookTable->addAskNew((qreal)(*itr)[1].GetDouble(), (qreal)(*itr)[2].GetDouble(), timestamp, flag);
			}
			delete doc;
			if (times + time_space < timestamp)
			{
				times = timestamp;
				break;
			}

		}
	}

	place = (size_t)_ftelli64(dumpFile);
	return orderBookTable;
}