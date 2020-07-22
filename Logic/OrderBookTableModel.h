#pragma once

#include <QAbstractTableModel>

#include "Order.h"
#include "Deals.h"

class OrderBookTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    int centerIndex;
    Deals* deals;
    QList<Order*> rows;

    OrderBookTableModel(QObject* parent = nullptr);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    // Интерфейс для добавления новых ордеров.
    void addBidNew(Order* newBid);
    void addAskNew(Order* newAsk);

    void updateTable();

private:
    QStringList headers;

    // Имплементация вставки ордеров в ордербук.
    void addBidToListNew(Order* newBid);
    void addAskToListNew(Order* newAsk);
    void deleteBidFromListNew(Order* newBid);
    void deleteAskFromListNew(Order* newAsk);
    void changeBidInListNew(Order* newBid);
    void changeAskInListNew(Order* newAsk);
};