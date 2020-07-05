#pragma once
#include <QAbstractTableModel>

class OrderBookTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:

    struct OrderBookTableStruct {
        QString asks;
        QString price;
        QString bids;
    };
    explicit OrderBookTableModel(QObject* parent = nullptr);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    void initOrderBookTableStruct();

private:
    QStringList headers;
    QList<OrderBookTableStruct> rows;
};