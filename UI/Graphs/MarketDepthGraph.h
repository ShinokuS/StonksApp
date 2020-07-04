#pragma once

#include "QtCharts/qchart.h"

using QtCharts::QChart;
using QtCharts::QAreaSeries;

class MarketDepthGraph : public QtCharts::QChart
{
public:
	MarketDepthGraph(QAreaSeries* bidsSeries, QAreaSeries* asksSeries);
};