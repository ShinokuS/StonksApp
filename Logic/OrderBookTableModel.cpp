#include "OrderBookTableModel.h"
#include "..\Utility\PriceAsQReal.h"

#include <QSize>
#include <QBrush>

#define price priceAsQReal(iter->first)
#define nOrders qreal(iter->second)

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
        case 0: return row.prices;
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

void OrderBookTableModel::initOrderBookTableStruct(OrderBook* orderBook)
{
    OrderBookTableStruct rowOrderBook;
    std::map<long long, int>* bids = &orderBook->bidsAmountForPrice;
    std::map<long long, int>* asks = &orderBook->asksAmountForPrice;
    
    OrderBookTableModel::centerIndex = 0;
    rowOrderBook.askMarker = true;
    auto iter = asks->end();
    do {
        iter--;
        rowOrderBook.prices = price;
        rowOrderBook.quantity = nOrders;
        rows.append(std::move(rowOrderBook));
        OrderBookTableModel::centerIndex++;
    } while (iter != asks->begin());
    rowOrderBook.askMarker = false;
    iter = bids->end();
    do {
        iter--;
        rowOrderBook.prices = price;
        rowOrderBook.quantity = nOrders;
        rows.append(std::move(rowOrderBook));
    } while (iter != bids->begin());
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