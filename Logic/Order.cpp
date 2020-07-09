#include "Order.h"

bool Order::operator<(const Order& other) {
    return this->price > other.price;
}