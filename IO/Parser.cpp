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
	search.resize(9);

	OrderBookTableModel* orderBookTable = new OrderBookTableModel; //Книжка для заполнения ордерами

	for (size_t i = 0; i < filesize; ++i)//Начинаем поиск по файлу
	{
		search[0] = search[1];
		search[1] = search[2];
		search[2] = search[3];
		search[3] = search[4];
		search[4] = search[5];
		search[5] = search[6];
		search[6] = search[7];
		search[7] = search[8];
		search[8] = fgetc(dumpFile);

		if (search == "timestamp")	//Если находим ключевое слово, начинаем считывать чистую json-строку
		{
			std::string json = "{\"timestamp"; //Инициализирую строку прочитанными символами,чтобы не перемещать указатель и считывать их снова.
			char ch;		//Не знаю, как лучше назвать переменную :-)	
			int right = 0;	//Количество }
			int left = 1;	//Количество {
			while (left != right)
			{
				ch = fgetc(dumpFile);//Считываем посимвольно json-строку для Document.
				++i;
				json += ch; //Формируем строку для Document.
				if (ch == '{')
				{
					++left;
				}
				else if (ch == '}')
				{
					++right;
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

			delete[] doc;
		}
	}

	return orderBookTable;
}
