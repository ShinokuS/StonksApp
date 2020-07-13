#pragma once

#include "QtCharts/qchart.h"
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QDateTime>

using QtCharts::QChart;
using QtCharts::QLineSeries;
using QtCharts::QDateTimeAxis;
using QtCharts::QValueAxis;

class LinePriceGraph : public QtCharts::QChart
{
public:
	LinePriceGraph(QLineSeries* lineSeries);
};