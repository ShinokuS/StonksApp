#include "PriceGraph.h"

PriceGraph::PriceGraph(QLineSeries* lineSeries)
{
    priceLineGraph(lineSeries);
}

void PriceGraph::priceLineGraph(QLineSeries* lineSeries)
{
    this->addSeries(lineSeries);
    this->createDefaultAxes();
    this->setTitle("Market price");
}