﻿#pragma once

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

    // интерфейс для добавления элементов
    void addBid(qreal price, qreal amount);
    void addAsk(qreal price, qreal amount);

private:
    QStringList headers;
};