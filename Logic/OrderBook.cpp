#include "OrderBook.h"

OrderBook::OrderBook()
{
    indexOfFirstVisibleElement = 0;
    countOfAsks = 0;
}

void OrderBook::addOrder(Order* newOrder)
{
    if (newOrder->flag == "new")
    {
        insertOrder(newOrder);
    }
    else if (newOrder->flag == "delete")
    {
        deleteOrder(newOrder);
    }
    else if (newOrder->flag == "change")
    {
        changeOrder(newOrder);
    }
}

void OrderBook::insertOrder(Order* newOrder)
{
    // Костыль из-за интерфейса STL
    Order reference = { newOrder->price };

    if (newOrder->isAsk && countOfAsks <= 10) countOfAsks++;
    if (countOfAsks > 10 && newOrder->isAsk) {
        indexOfFirstVisibleElement++;
    }
    // Бинарный поиск позиции с ценой не меньше указанной
    auto iter = std::lower_bound(rows.begin(), rows.end(), reference,
        [reference](Order* element, const Order reference) { return element->price > reference.price; });

    rows.insert(iter, newOrder);
}

void OrderBook::deleteOrder(Order* newOrder)
{
    // Костыль из-за интерфейса STL
    Order reference = { newOrder->price, 0, false };

    if (countOfAsks > 10 && newOrder->isAsk) {
        indexOfFirstVisibleElement--;
    }
    // Бинарный поиск позиции с ценой не меньше указанной
    auto iter = std::lower_bound(rows.begin(), rows.end(), reference,
        [reference](Order* element, const Order reference) { return element->price > reference.price; });

    // Проверку на то, что цена пришла валидная и точно имеющаяся в списке, не делаю!
    // Как и на то, совпадает ли маркер бида/аска.

    delete* iter;
    rows.erase(iter);
}

void OrderBook::changeOrder(Order* newOrder)
{
    // Костыль из-за интерфейса STL
    Order reference = { newOrder->price, 0, false };

    // Бинарный поиск позиции с ценой не меньше указанной
    auto iter = std::lower_bound(rows.begin(), rows.end(), reference,
        [reference](Order* element, const Order reference) { return element->price > reference.price; });

    // Проверку на то, что цена пришла валидная и точно имеющаяся в списке, не делаю!
    // Как и на то, совпадает ли маркер бида/аска.

    (*iter)->quantity = newOrder->quantity;
    delete newOrder;
}