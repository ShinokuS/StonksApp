#pragma once

#include <map>
#include <set>
#include <vector>
#include <utility>

#include "LimitOrder.h"

// Выдаёт полученные парсером данные отдельно по инструментам.

class LimitOrdersData
{
public:

	LimitOrdersData(std::vector<LimitOrder> *dataFromParser);
	std::set<long long>& getInstrumentIdList();
	std::pair<LimitOrder*, LimitOrder*> getOrdersRangeForInstrument(long long instrumentId);

private:

	std::vector<LimitOrder> * data;
	std::set<long long> instrumentIdList;
	std::map<long long, std::pair<LimitOrder*, LimitOrder*>> instrumentRanges;

	void saveInstrumentRanges();
	void sortInstrumentRangesByPrice();
};