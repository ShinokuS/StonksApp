#include "PriceGraph.h"

LinePriceGraph::LinePriceGraph(QLineSeries* lineSeries)
{
    this->legend()->hide();
    this->addSeries(lineSeries);
    this->createDefaultAxes();
    this->setTitle("Market price");
}
