#pragma once

#include <QAbstractTableModel>

struct Order
{
    qreal price;
    qreal quantity;
    bool askMarker;
};