#pragma once

// Простая структура для хранения информации о лимитном ордере,
// достаточной чтобы нарисовать график глубины рынка ("стакан").

struct LimitOrder
{
	long long priceInCents;
	int quantity;
};