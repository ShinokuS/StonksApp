#include "OrderBookTableModel.h"

#include <QSize>
#include <QBrush>

OrderBookTableModel::OrderBookTableModel(QObject* parent)
    : QAbstractTableModel(parent)
{
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
        case 0: return row.price;
        case 1: return row.quantity;
        }
    case Qt::TextAlignmentRole:
        return Qt::AlignCenter;
    case Qt::BackgroundRole:
        if (row.askMarker) {
            return QBrush(0xff6161);
        }
        else return QBrush(0x7ce670);
    }
    return QVariant();
}

int randomBetween(int begin, int end)
{
    return begin + rand() % (end - begin);
}

OrderBookTableModel* OrderBookTableModel::getRandomInstance(unsigned int seed)
{
    // настройки рандома
    srand(seed);
    int nBids = randomBetween(20, 100);
    int nAsks = randomBetween(20, 100);
    long long minBidPrice = 100;
    long long minAskPrice = 10000;
    long long maxAskPrice = 20000;
    int maxAmountInOrder = 10000;

    auto randomOrderBook = new OrderBookTableModel;
    randomOrderBook->centerIndex = 0;

    for (int i = 0; i < nBids; i++) {
        qreal price = randomBetween(minBidPrice, minAskPrice);
        price /= 100; // перевод из копеек
        qreal quantity = randomBetween(1, maxAmountInOrder);
        randomOrderBook->addBid(price, quantity);
    }

    for (int i = 0; i < nAsks; i++) {
        qreal price = randomBetween(minAskPrice, maxAskPrice);
        price /= 100; // перевод из копеек
        qreal quantity = randomBetween(1, maxAmountInOrder);
        randomOrderBook->addAsk(price, quantity);
    }

    return randomOrderBook;
}

int OrderBookTableModel::returnCenterIndex()
{
    return OrderBookTableModel::centerIndex;
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

// НИЖЕ, ОБА МЕТОДА ВСТАВКИ ТРАТЯТ ПО O(n) НА ВСТАВКУ!
// НАДЕЮСЬ, ЭТО ВРЕМЕННОЕ РЕШЕНИЕ!

void OrderBookTableModel::addBid(qreal price, qreal amount)
{
    // Если добавляемый ордер вызывает сделки, проводим их.
    Order orderToAdd = { price, amount, false };
    makeDealsIfNeededFor(orderToAdd);

    // Если ордер при этом полностью исполнился, то его не надо будет заносить в список.
    if (orderToAdd.quantity == 0) {
        return;
    }

    // Если нет, то собственно заносим:

    Order reference = { price, 0, false }; // костыль из-за интерфейса STL

    // бинарный поиск позиции с ценой не меньше указанной
    auto iter = std::lower_bound(rows.begin(), rows.end(), reference,
        [reference](Order& element, const Order reference) { return element.price > reference.price; });

    // если это будет новая самая дешёвая позиция, то создаём её в конце списка
    if (iter == rows.end()) {
        rows.append(std::move(orderToAdd));
    }
    // если не самая, но конкретно такой цены нет, то создаём её в нужном месте
    else if (iter->price < price) {
        rows.insert(iter, std::move(orderToAdd));
    }
    // если позиция есть, то приплюсовываем туда количество
    else {
        iter->quantity += amount;
    }
}

void OrderBookTableModel::addAsk(qreal price, qreal amount)
{
    // Если добавляемый ордер вызывает сделки, проводим их.
    Order orderToAdd = { price, amount, true };
    makeDealsIfNeededFor(orderToAdd);

    // Если ордер при этом полностью исполнился, то его не надо будет заносить в список.
    if (orderToAdd.quantity == 0) {
        return;
    }

    // Если нет, то собственно заносим:

    Order reference = { price, 0, true }; // костыль из-за интерфейса STL

    // бинарный поиск позиции с ценой не меньше указанной
    auto iter = std::lower_bound(rows.begin(), rows.end(), reference,
        [reference](Order& element, const Order reference) { return element.price > reference.price; });

    // если позиции с такой ценой нет, то создаём её в нужном месте
    if (iter->price < price) {
        rows.insert(iter, std::move(orderToAdd));
        centerIndex++;
    }
    // если позиция есть, то приплюсовываем туда количество
    else {
        iter->quantity += amount;
    }
}


void OrderBookTableModel::makeDealsIfNeededFor(Order& newOrder)
{

}