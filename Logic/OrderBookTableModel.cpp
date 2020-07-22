#include "OrderBookTableModel.h"


#include <QSize>
#include <QBrush>

OrderBookTableModel::OrderBookTableModel(QObject* parent)
    : QAbstractTableModel(parent)
{
    centerIndex = 0;
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

void OrderBookTableModel::updateTable()
{
    emit layoutAboutToBeChanged();
    emit layoutChanged();
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

void OrderBookTableModel::addBidNew(Order* newBid)
{
    if (newBid->flag == "new")
    {
        addBidToListNew(newBid);
    }
    else if (newBid->flag == "delete")
    {
        deleteBidFromListNew(newBid);
    }
    else if (newBid->flag == "change")
    {
        changeBidInListNew(newBid);
    }
}

void OrderBookTableModel::addAskNew(Order* newAsk)
{
    if (newAsk->flag == "new")
    {
        addAskToListNew(newAsk);
    }
    else if (newAsk->flag == "delete")
    {
        deleteAskFromListNew(newAsk);
    }
    else if (newAsk->flag == "change")
    {
        changeAskInListNew(newAsk);
    }
}

void OrderBookTableModel::addBidToListNew(Order* newBid)
{
    Order reference = { newBid->price, 0, false }; // костыль из-за интерфейса STL

    // бинарный поиск позиции с ценой не меньше указанной
    auto iter = std::lower_bound(rows.begin(), rows.end(), reference,
        [reference](Order* element, const Order reference) { return element->price > reference.price; });

        rows.insert(iter, newBid);
    
}

void OrderBookTableModel::addAskToListNew(Order* newAsk)
{
    Order reference = { newAsk->price, 0, true }; // костыль из-за интерфейса STL

    // бинарный поиск позиции с ценой не меньше указанной
    auto iter = std::lower_bound(rows.begin(), rows.end(), reference,
        [reference](Order* element, const Order reference) { return element->price > reference.price; });

        rows.insert(iter, newAsk);
        centerIndex++;
}

void OrderBookTableModel::deleteBidFromListNew(Order* newBid)
{
    for (auto it = rows.begin(); it != rows.end();++it)
    {
        if (((*it)->price == newBid->price)&&((*it)->isAsk == newBid->isAsk))
        {
            delete *it;
            rows.erase(it);
            return;
        }
    }
}

void OrderBookTableModel::deleteAskFromListNew(Order* newAsk)
{
    for (auto it = rows.begin(); it != rows.end(); ++it)
    {
        if (((*it)->price == newAsk->price) && ((*it)->isAsk == newAsk->isAsk))
        {
            delete *it;
            rows.erase(it);
            --centerIndex;
            return;
        }
    }
}

void OrderBookTableModel::changeBidInListNew(Order* newBid)
{
    for (auto it = rows.begin(); it != rows.end(); ++it)
    {
        if (((*it)->price == newBid->price) && ((*it)->isAsk == newBid->isAsk))
        {
            delete *it;
            *it = newBid;
            return;
        }
    }
}

void OrderBookTableModel::changeAskInListNew(Order* newAsk)
{
    for (auto it = rows.begin(); it != rows.end(); ++it)
    {
        if (((*it)->price == newAsk->price) && ((*it)->isAsk == newAsk->isAsk))
        {
            delete *it;
            *it = newAsk;
            return;
        }
    }
}