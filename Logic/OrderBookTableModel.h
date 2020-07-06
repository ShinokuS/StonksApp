#pragma once
#include <QAbstractTableModel>
#include "OrderBook.h"

class OrderBookTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:

    struct OrderBookTableStruct {
        qreal prices;
        qreal quantity;
        bool askMarker;
    };
    int centerIndex;

    OrderBookTableModel(QObject* parent = nullptr);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    void initOrderBookTableStruct(OrderBook *orderBook);
    int returnCenterIndex();

private:
    QStringList headers;
    QList<OrderBookTableStruct> rows;
};