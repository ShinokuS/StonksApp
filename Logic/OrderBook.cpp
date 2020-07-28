#include "OrderBook.h"
#include "SmallOrderBookTableModel.h"

OrderBook::OrderBook(std::vector<Order>* ordersSource)
{
    indexOfFirstVisibleElement = 0;
    countOfAsks = 0;
    activeOrderIndexInSource = -1;
    this->ordersSource = ordersSource;
    orders.reserve(10000); // Ну сколько там позиций по цене может быть?
}

bool OrderBook::canLoadNextOrderFromSource()
{
    return activeOrderIndexInSource < (int)ordersSource->size() - 1;
}

void OrderBook::loadNextOrderFromSource()
{
    activeOrderIndexInSource++;
    addOrder(&(*ordersSource)[activeOrderIndexInSource]);
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
    for (auto iter = orders.begin(); iter != orders.end(); ++iter)
    {
        // Проверяем и маркер аска, потому что в моменты между сделками бывает так,
        // что на одну цену весит и бид, и аск. И нужно выбрать из них нужное.
        if ((newOrder->price == (*iter)->price) && (newOrder->isAsk == (*iter)->isAsk))
        {
            orders.erase(iter);
            break;
        }
    }
    
    if (newOrder->isAsk) {
        countOfAsks--;
        if (countOfAsks > SmallOrderBookTableModel::MAX_VISIBLE_ASKS_AMOUNT) {
            indexOfFirstVisibleElement--;
        }
    }
}

void OrderBook::changeOrder(Order* newOrder)
{
    for (auto iter = orders.begin(); iter != orders.end(); ++iter)
    {
        // Проверяем и маркер аска, потому что в моменты между сделками бывает так,
        // что на одну цену весит и бид, и аск. И нужно выбрать из них нужное.
        if ((newOrder->price == (*iter)->price) && (newOrder->isAsk == (*iter)->isAsk))
        {
            (*iter)->quantity = newOrder->quantity;
            break;
        }
    }
}