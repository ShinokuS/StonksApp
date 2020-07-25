#include "BotGraph.h"

BotGraph::BotGraph(QVector <double> timeBuy, QVector <double> priceBuy,
				QVector <double> timeSell, QVector <double> priceSell, PriceGraph* priceGraph)
{
	QVector<QCPScatterStyle::ScatterShape> shapes;
	shapes << QCPScatterStyle::ssTriangle;
	shapes << QCPScatterStyle::ssTriangleInverted;

	priceGraph->addGraph();
	priceGraph->graph(1)->setLineStyle(QCPGraph::lsNone);
	priceGraph->graph(1)->setScatterStyle(QCPScatterStyle(shapes.at(0), 15));
	priceGraph->addGraph();
	priceGraph->graph(2)->setLineStyle(QCPGraph::lsNone);
	priceGraph->graph(2)->setScatterStyle(QCPScatterStyle(shapes.at(1), 15));

	QPen pen;
	pen.setColor(0x059605);
	priceGraph->graph(1)->setPen(pen);
	pen.setColor(0x940000);
	priceGraph->graph(2)->setPen(pen);

	priceGraph->graph(1)->setData(timeBuy, priceBuy);
	priceGraph->graph(2)->setData(timeSell, priceSell);
}