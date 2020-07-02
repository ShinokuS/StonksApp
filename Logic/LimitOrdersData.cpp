#include <algorithm>

#include "LimitOrdersData.h"

// public:

LimitOrdersData::LimitOrdersData(std::vector<LimitOrder> * dataFromParser)
{
	data = dataFromParser;
	instrumentIdList = std::set<long long>();
	instrumentRanges = std::map<long long, std::pair<LimitOrder*, LimitOrder*>>();

	// сортировка данных по возрастанию instrumentId,
	// чтобы ордеры были сгруппированы по инструментам
	std::sort(data->begin(), data->end(),
			[](LimitOrder& left, LimitOrder& right)
			{return left.instrumentId < right.instrumentId; });

	saveInstrumentRanges();
	sortInstrumentRangesByPrice();
}

std::set<long long>& LimitOrdersData::getInstrumentIdList()
{
	return instrumentIdList;
}

std::pair <LimitOrder*, LimitOrder*> LimitOrdersData::getOrdersRangeForInstrument(long long instrumentId)
{
	return instrumentRanges[instrumentId];
}

// private:

void LimitOrdersData::saveInstrumentRanges()
{
	long long currentInstrument;
	LimitOrder* instrumentBegin = &*data->begin();
	LimitOrder* instrumentEnd;

	// Для групп ордеров на каждый инструмент отдельно:
	for (/*     */; instrumentBegin < &*data->end(); instrumentBegin = instrumentEnd) {
		currentInstrument = instrumentBegin->instrumentId;

		// Находим конец группы
		instrumentEnd = instrumentBegin + 1;
		while (instrumentEnd <= &*data->end() && instrumentEnd->instrumentId == currentInstrument) {
			instrumentEnd++;
		}

		// Сохранение группы для обращения к ней потом по id инструмента.
		instrumentIdList.insert(currentInstrument);
		instrumentRanges.insert({ currentInstrument, {instrumentBegin, instrumentEnd} });
	}
}

void LimitOrdersData::sortInstrumentRangesByPrice()
{
	for (long long id : instrumentIdList) {
		LimitOrder* instrumentBegin = instrumentRanges[id].first;
		LimitOrder* instrumentEnd = instrumentRanges[id].second;
		
		std::sort(instrumentBegin, instrumentEnd,
			[](LimitOrder& left, LimitOrder& right)
			{return left.priceInCents < right.priceInCents; });
	}
}