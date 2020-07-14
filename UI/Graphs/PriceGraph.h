#pragma once

#include "../../Logic/QCustomPlot/qcustomplot.h"

#include <QDateTime>

class LinePriceGraph : public QCustomPlot
{
public:
	LinePriceGraph(QVector <double> time, QVector <double> price);
};