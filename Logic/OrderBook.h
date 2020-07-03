#pragma once

#include <map>
#include <string>

// Хранит всю информацию об ордерах, нужную для отрисовки стакана

class OrderBook
{
public:

	std::string instrument;

	std::map<long long, int> asksAmountForPrice;
	std::map<long long, int> bidsAmountForPrice;
	
	long long minPrice;
	long long midPrice;
	long long maxPrice;
};

