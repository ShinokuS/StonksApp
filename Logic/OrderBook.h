#pragma once

#include "Deals.h"
#include "Order.h"

class OrderBook
{
public:
    int indexOfFirstVisibleElement;
    int countOfAsks;
    Deals* deals;
    QList<Order*> orders;

    OrderBook();

    // Интерфейс для добавления новых ордеров.
    void addOrder(Order* newOrder);

    void updateTable();

private:

    // Имплементация операций над ордерами в ордербуке.
    void insertOrder(Order* newOrder);
    void deleteOrder(Order* newOrder);
    void changeOrder(Order* newOrder);
};

