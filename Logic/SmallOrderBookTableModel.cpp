#include "SmallOrderBookTableModel.h"

#include <QSize>
#include <QBrush>

SmallOrderBookTableModel::SmallOrderBookTableModel(QObject* parent)
    : QAbstractTableModel(parent)
{
    headers << "Price" << "Quantity";
}

QVariant SmallOrderBookTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section < headers.size()) {
        return headers.at(section);
    }
    return {};
}

int SmallOrderBookTableModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;
    return rows.size();
}

int SmallOrderBookTableModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;
    return headers.size();
}

QVariant SmallOrderBookTableModel::data(const QModelIndex& index, int role) const
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

bool SmallOrderBookTableModel::setData(const QModelIndex& index, const QVariant&, int role)
{
    if (!index.isValid()) return false;
    return true;
}

Qt::ItemFlags SmallOrderBookTableModel::flags(const QModelIndex& index) const
{
    return QAbstractTableModel::flags(index) | Qt::ItemIsUserCheckable;
}

void SmallOrderBookTableModel::changeData(OrderBookTableModel* orderBook)
{
    int countOfVisibleOrders = 20;

    if(orderBook->rowCount() <= countOfVisibleOrders){
        rows = orderBook->rows;
    }
    else {
        for (int i = 0;i < countOfVisibleOrders; i++) {
            if (rowCount()<countOfVisibleOrders) {
                rows.append(orderBook->rows[i + orderBook->indexOfFirstVisibleElement]);
            }
            else {
                rows.replace(i, orderBook->rows[i + orderBook->indexOfFirstVisibleElement]);
            }
        }
    }
}

void SmallOrderBookTableModel::updateTable()
{
    emit layoutAboutToBeChanged();
    emit layoutChanged();
}