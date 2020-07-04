#include "PriceAsQReal.h"

qreal priceAsQReal(int priceInCents)
{
	return qreal(priceInCents) * 0.01;
}