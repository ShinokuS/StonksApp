#pragma once

#include "QtCharts/qchart.h"
#include <QtCharts/QLineSeries>

using QtCharts::QChart;
using QtCharts::QLineSeries;

class LinePriceGraph : public QtCharts::QChart
{
public:
	LinePriceGraph(QLineSeries* lineSeries);
};