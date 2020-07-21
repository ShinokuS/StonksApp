#pragma once

#include <string>

#include <QAbstractTableModel>

struct Order
{
    qreal price;
    qreal quantity;
    bool isAsk;
    time_t time;
    std::string flag;
};