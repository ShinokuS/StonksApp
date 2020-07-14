#define _CRT_SECURE_NO_WARNINGS
#include "string"
//#include "IO/rapidjson/rapidjson.h"
#include "IO/rapidjson/document.h"
#include "Logic/OrderBookTableModel.h"
#include "IO/Parser.h"


OrderBookTableModel* Parser::Parse(std::string fileName, std::string instrumentName)
{
	FILE* dumpFile = fopen(fileName.c_str(), "rb");

	_fseeki64(dumpFile, 0, SEEK_END);
	size_t filesize = (size_t)_ftelli64(dumpFile);//определяем размер файла.
	_fseeki64(dumpFile, 0, SEEK_SET);
	
	std::string search;//Буффер для поиска ключевых слов
	const std::string keyWord = "book." + instrumentName; 
	search.resize(keyWord.size());

	OrderBookTableModel* orderBookTable = new OrderBookTableModel; //Книжка для заполнения ордерами

	int counter = 0;

	for (size_t i = 0; i < filesize; ++i)//Начинаем поиск по файлу
	{
		for (int j = 0; j < search.size()-1; ++j)
		{
			search[j] = search[j+1];
		}
		search.back() = fgetc(dumpFile);
		if (search == (keyWord))	//Если находим ключевое слово, начинаем считывать чистую json-строку
		{
			if (counter == 5)
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
				if (flag == "new")
				{
					//Здесь должнен быть метод добавления ордера(bid)
				}
				else if (flag == "delete")
				{
					//Здесь должнен быть метод удаления ордера(bid)
				}
				else if (flag == "change")
				{
					//Здесь должнен быть метод изменнения ордера(bid)
				}
			} 

			for (auto itr = (*doc)["asks"].Begin(); itr != (*doc)["asks"].End(); ++itr)	//Прогоняемся по массиву asks для заполнения книжки
			{
				std::string flag = (*itr)[0].GetString();
				if (flag == "new")
				{
					//Здесь должнен быть метод добавления ордера(ask)
				}
				else if (flag == "delete")
				{
					//Здесь должнен быть метод удаления ордера(ask)
				}
				else if (flag == "change")
				{
					//Здесь должнен быть метод изменнения ордера(ask)
				}
			}
			delete doc;
		}
	}

	return orderBookTable;
}
