#define _CRT_SECURE_NO_WARNINGS
#include "string"
#include "IO/rapidjson/rapidjson.h"
#include "IO/rapidjson/document.h"
#include "Logic/OrderBookTableModel.h"
#include "IO/Parser.h"
using namespace rapidjson;

OrderBookTableModel* Parser::Parse(std::string fileName, std::string instrumentName)
{
	FILE* DumpFile = fopen(fileName.c_str(), "rb");
	_fseeki64(DumpFile, 0, SEEK_END);
	size_t filesize = (size_t)malloc(_ftelli64(DumpFile));
	_fseeki64(DumpFile, 0, SEEK_SET);

	std::string buffer;
	buffer.resize(9);

	OrderBookTableModel* orderBookTable = new OrderBookTableModel;
	for (size_t i = 0; i < filesize; ++i)
	{
		buffer[0] = buffer[1];
		buffer[1] = buffer[2];
		buffer[2] = buffer[3];
		buffer[3] = buffer[4];
		buffer[4] = buffer[5];
		buffer[5] = buffer[6];
		buffer[6] = buffer[7];
		buffer[7] = buffer[8];
		buffer[8] = fgetc(DumpFile);
		if (buffer == "timestamp")
		{
			std::string json_str = "{\"timestamp";
			char ch;
			int right = 0;
			int left = 1;
			while (left != right)
			{
				ch = fgetc(DumpFile);
				++i;
				json_str += ch;
				if (ch == '{')
				{
					++left;
				}
				if (ch == '}')
				{
					++right;
				}
			}
			Document* d = new Document;
			d->Parse(json_str.c_str());
			//time_t timestamp = (*d)["timestamp"].GetInt64();
			//for (auto itr = (*d)["bids"].Begin(); itr != (*d)["bids"].End();++itr)
			//{
			//	
			//}

		}
	}

	return 0;
}
