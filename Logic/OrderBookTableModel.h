#pragma once
#include <QAbstractTableModel>

class OrderBookTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:

    struct Order {
        qreal price;
        qreal quantity;
        bool askMarker;
        bool operator==(const Order& other) {
            if (this->price != other.price) {
                return false;
            }
            if (this->quantity != other.quantity) {
                return false;
            }
            if (this->askMarker != other.askMarker) {
                return false;
            }
            return true;
        }
        bool operator<(const Order& other) {
            return this->price > other.price;
        }
    };
    int centerIndex;
    QList<Order> rows;

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