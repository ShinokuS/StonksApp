#pragma once

#include "../../Logic/QCustomPlot/qcustomplot.h"
#include "PriceGraph.h"

#include <QDateTime>

class BotGraph : public QCustomPlot
{
public:
	BotGraph(QVector <double> time, QVector <double> price, PriceGraph* priceGraph);
};
