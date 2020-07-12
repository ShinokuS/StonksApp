#pragma once

#include "QtCharts/qchart.h"
#include <QtCharts/QLineSeries>

using QtCharts::QChart;
using QtCharts::QLineSeries;

class PriceGraph : public QtCharts::QChart
{
public:
	PriceGraph(QLineSeries* lineSeries);

private:
	void priceLineGraph(QLineSeries* lineSeries);
};