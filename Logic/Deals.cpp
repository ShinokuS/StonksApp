#include "Deals.h"

Deals::Deals(std::vector<Order>* dealsSource)
{
	this->dealsSource = dealsSource;
	dealsForPriceGraph.reserve(dealsSource->capacity());
	activeDealIndexInSource = -1;
}

bool Deals::canLoadNextDealFromSource()
{
	return activeDealIndexInSource < (int)dealsSource->size() - 1;
}

void Deals::loadNextDealFromSource()
{
	activeDealIndexInSource++;
	dealsForPriceGraph.append(&(*dealsSource)[activeDealIndexInSource]);
}

bool Deals::canLoadMoreFromTheSameFrame()
{
	if (!canLoadNextDealFromSource()) {
		return false; // но вдруг здесь надо дождаться парсера на том потоке?
	}
	else {
		auto timeframe = this->getLastDeal()->time;
		return (*dealsSource)[activeDealIndexInSource + 1].time == timeframe;
	}
}

Order* Deals::getLastDeal()
{
	return dealsForPriceGraph.back();
}