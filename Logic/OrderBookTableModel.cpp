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

void OrderBookTableModel::getTestOrderBookTable(unsigned int seed) 
{
    srand(seed);
    int nBids = randomBetween(20, 100);
    int nAsks = randomBetween(20, 100);
    long long minBidPrice = 100;
    long long minAskPrice = 10000;
    long long maxAskPrice = 20000;
    int maxAmountInOrder = 10000;

    OrderBookTableModel::centerIndex = 0;

    for (int i = 0; i < nBids; i++) {
        qreal price = randomBetween(minBidPrice, minAskPrice);
        price /= 100; // перевод из копеек
        qreal quantity = randomBetween(1, maxAmountInOrder);
        addBid(price, quantity);
    }

    for (int i = 0; i < nAsks; i++) {
        qreal price = randomBetween(minAskPrice, maxAskPrice);
        price /= 100; // перевод из копеек
        qreal quantity = randomBetween(1, maxAmountInOrder);
        addAsk(price, quantity);
    }

    std::sort(rows.begin(), rows.end());
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

void OrderBookTableModel::addBid(qreal price, qreal amount)
{
    Order order = { price, amount, false };
    rows.append(std::move(order));
}

void OrderBookTableModel::addAsk(qreal price, qreal amount)
{
    Order order = { price, amount, true };
    rows.append(std::move(order));
    centerIndex++;
}