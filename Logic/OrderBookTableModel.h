#pragma once

#include <QAbstractTableModel>

#include "Order.h"

class OrderBookTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    int centerIndex;
    QList<Order> rows;

    OrderBookTableModel(QObject* parent = nullptr);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    static OrderBookTableModel* getRandomInstance(unsigned int seed = time(0));
    int returnCenterIndex();

    // Интерфейс для добавления новых ордеров.
    void addBid(qreal price, qreal amount);
    void addAsk(qreal price, qreal amount);

private:
    QStringList headers;

    // Проводит автосделки по добавляемому ордеру.
    void makeDealsIfNeededFor(Order& newOrder);

    bool canMakeDealBetween(Order& one, Order& other);

    // Имплементация вставки ордеров в ордербук.
    // Вставляют сразу на место, но за O(n). Мб это можно будет улучшить.
    // Не заменой rows на связный список! Ибо доступ по индексу тоже не должен тупить.
    void addBidToList(Order& newBid);
    void addAskToList(Order& newAsk);
};