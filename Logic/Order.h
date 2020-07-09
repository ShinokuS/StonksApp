#pragma once

#include <QAbstractTableModel>

struct Order
{
    qreal price;
    qreal quantity;
    bool askMarker;

    bool operator<(const Order& other);
};