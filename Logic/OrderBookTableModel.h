#pragma once
#include <QAbstractTableModel>
#include <chrono>

#include "OrderBook.h"

class OrderBookTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:

    struct OrderBookTableStruct {
        qreal prices;
        qreal quantity;
        bool askMarker;
        bool operator==(const OrderBookTableStruct& s) {
            if (this->prices != s.prices) {
                return false;
            }
            if (this->quantity != s.quantity) {
                return false;
            }
            if (this->askMarker != s.askMarker) {
                return false;
            }
            return true;
        }
        bool operator<(const OrderBookTableStruct& s) {
            return this->prices> s.prices;
        }
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

    void getTestOrderBookTable(unsigned int seed = time(0));

    int returnCenterIndex();

private:
    QStringList headers;
    QList<OrderBookTableStruct> rows;

};