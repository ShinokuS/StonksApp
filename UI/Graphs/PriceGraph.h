#pragma once

#include "QtCharts/qchart.h"
#include <QtCharts/QLineSeries>

using QtCharts::QChart;
using QtCharts::QLineSeries;

class PriceGraph
{
	void LinePriceGraph(QLineSeries* lineSeries);
};