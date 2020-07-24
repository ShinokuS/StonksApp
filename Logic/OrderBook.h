#pragma once

#include <vector>

#include "Order.h"

class OrderBook
{
public:
    int indexOfFirstVisibleElement;
    int countOfAsks;
    int activeOrderIndexInSource;
    QList<Order*> orders;

    OrderBook(std::vector<Order*>* ordersSource);

    // Интерфейс для добавления новых ордеров.
    void addOrder(Order* newOrder);

    bool canLoadNextOrderFromSource();
    void loadNextOrderFromSource();

private:

    std::vector<Order*>* ordersSource;

    // Имплементация операций над ордерами в ордербуке.
    void insertOrder(Order* newOrder);
    void deleteOrder(Order* newOrder);
    void changeOrder(Order* newOrder);
};

