#pragma once

// Простая структура для хранения информации о лимитном ордере,
// достаточной чтобы нарисовать график глубины рынка ("стакан").

struct LimitOrder
{
	enum DealType {
		BUY = 0,
		SELL = 1
	};

	int instrumentId;
	int priceInCents;
	int quantity;
	DealType dealType;
};