#include "OrderBookTableModel.h"

#include <QSize>
#include <QBrush>
#include <QScrollBar>

OrderBookTableModel::OrderBookTableModel(QObject* parent)
    : QAbstractTableModel(parent)
{
    headers << "Bids" << "Price" << "Asks";
    initOrderBookTableStruct();
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
    switch(role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        switch (index.column()) {
        case 0: return row.bids;
        case 1: return row.price;
        case 2: return row.asks;
        }
    case Qt::TextAlignmentRole:
        return Qt::AlignCenter;
    case Qt::BackgroundRole:
        if (row.bids == "") {
            return QBrush(Qt::red);
        }
        else return QBrush(Qt::green);
    }
    
    return QVariant();
}

void OrderBookTableModel::initOrderBookTableStruct()
{
    for (int i = 1; i < 31; ++i) {
        OrderBookTableStruct rowOrderBook;
        if (i > 15) {
            rowOrderBook.bids = QString("bid");
            rowOrderBook.asks = QString("");
        }
        else {
            rowOrderBook.bids = QString("");
            rowOrderBook.asks = QString("ask");
        }
        rowOrderBook.price = QString("price");
        rows.append(std::move(rowOrderBook));
    }
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