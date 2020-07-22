#include "OrderBookTableModel.h"


#include <QSize>
#include <QBrush>

OrderBookTableModel::OrderBookTableModel(QObject* parent)
    : QAbstractTableModel(parent)
{
    indexOfFirstVisibleElement = 0;
    countOfAsks = 0;
    headers << "Price" << "Quantity";
}

QVariant OrderBookTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section < headers.size()) {
        return headers.at(section);
    }
    return {};
}

int OrderBookTableModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;
        return rows.size();
}

int OrderBookTableModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;
        return headers.size();
}

QVariant OrderBookTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    auto row = rows.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        switch (index.column()) {
        case 0: return row->price;
        case 1: return row->quantity;
        }
    case Qt::TextAlignmentRole:
        return Qt::AlignCenter;
    case Qt::BackgroundRole:
        if (row->isAsk) {
            return QBrush(0xff6161);
        }
        else return QBrush(0x7ce670);
    }
    return QVariant();
}

bool OrderBookTableModel::setData(const QModelIndex& index, const QVariant&, int role)
{
    if (!index.isValid()) return false;
    return true;
}

Qt::ItemFlags OrderBookTableModel::flags(const QModelIndex& index) const
{
    return QAbstractTableModel::flags(index) | Qt::ItemIsUserCheckable;
}

void OrderBookTableModel::addOrder(Order* newOrder)
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

void OrderBookTableModel::insertOrder(Order* newOrder)
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

void OrderBookTableModel::deleteOrder(Order* newOrder)
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

    delete *iter;
    rows.erase(iter);
}

void OrderBookTableModel::changeOrder(Order* newOrder)
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