#include "OrderBook.h"
#include "SmallOrderBookTableModel.h"

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

    // Бинарный поиск позиции с ценой не меньше указанной
    auto iter = std::lower_bound(orders.begin(), orders.end(), reference,
        [reference](Order* element, const Order reference) { return element->price > reference.price; });

    orders.insert(iter, newOrder);

    if (newOrder->isAsk) {
        countOfAsks++;
        if (countOfAsks > SmallOrderBookTableModel::MAX_VISIBLE_ASKS_AMOUNT) {
            indexOfFirstVisibleElement++;
        }
    }
}

void OrderBook::deleteOrder(Order* newOrder)
{
    // Костыль из-за интерфейса STL
    Order reference = { newOrder->price, 0, false };

    // Бинарный поиск позиции с ценой не меньше указанной
    auto iter = std::lower_bound(orders.begin(), orders.end(), reference,
        [reference](Order* element, const Order reference) { return element->price > reference.price; });

    // Проверку на то, что цена пришла валидная и точно имеющаяся в списке, не делаю!
    // Как и на то, совпадает ли маркер бида/аска.

    delete* iter;
    orders.erase(iter);

    if (newOrder->isAsk) {
        countOfAsks--;
        if (countOfAsks > SmallOrderBookTableModel::MAX_VISIBLE_ASKS_AMOUNT) {
            indexOfFirstVisibleElement--;
        }
    }
}

void OrderBook::changeOrder(Order* newOrder)
{
    // Костыль из-за интерфейса STL
    Order reference = { newOrder->price, 0, false };

    // Бинарный поиск позиции с ценой не меньше указанной
    auto iter = std::lower_bound(orders.begin(), orders.end(), reference,
        [reference](Order* element, const Order reference) { return element->price > reference.price; });

    // Проверку на то, что цена пришла валидная и точно имеющаяся в списке, не делаю!
    // Как и на то, совпадает ли маркер бида/аска.

    (*iter)->quantity = newOrder->quantity;
    delete newOrder;
}