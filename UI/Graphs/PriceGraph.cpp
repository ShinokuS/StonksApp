#include "PriceGraph.h"

void PriceGraph::LinePriceGraph(QLineSeries* lineSeries)
{
    this->addSeries(lineSeries);
    this->createDefaultAxes();
    this->setTitle("Market price");
}