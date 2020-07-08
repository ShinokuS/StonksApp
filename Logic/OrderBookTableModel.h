#pragma once
#include <QAbstractTableModel>

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
    QList<OrderBookTableStruct> rows;

    OrderBookTableModel(QObject* parent = nullptr);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    void getTestOrderBookTable(unsigned int seed = time(0));
    int returnCenterIndex();

private:
    QStringList headers;
};