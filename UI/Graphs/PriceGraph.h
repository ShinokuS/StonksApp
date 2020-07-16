#pragma once

#include "../../Logic/QCustomPlot/qcustomplot.h"

#include <QDateTime>

class PriceGraph : public QCustomPlot
{
public:
	PriceGraph(QVector <double> time, QVector <double> price);
};