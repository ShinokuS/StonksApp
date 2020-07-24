#include "MarketDepthGraph.h"

MarketDepthGraph::MarketDepthGraph(QVector <double> priceAsk, QVector <double> quantityAsk,
                                QVector <double> priceBid, QVector <double> quantityBid)
{
    //this->setInteraction(QCP::iRangeZoom, true);
    //this->setInteraction(QCP::iRangeDrag, true);
    this->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    this->yAxis->setTickLabelFont(QFont(QFont().family(), 8));

    this->xAxis->setAutoTickStep(true);

    this->xAxis2->setVisible(true);
    this->yAxis2->setVisible(true);
    this->xAxis2->setTicks(false);
    this->yAxis2->setTicks(false);
    this->xAxis2->setTickLabels(false);
    this->yAxis2->setTickLabels(false);
    this->yAxis->setTickLabels(false);

    this->addGraph();
    this->graph(0)->setPen(QPen(QColor(0x940000), 1.5));
    this->graph(0)->setBrush(QBrush(0xff6161));
    this->addGraph();
    this->graph(1)->setPen(QPen(QColor(0x059605), 1.5));
    this->graph(1)->setBrush(QBrush(0x7ce670));

    this->graph(0)->setLineStyle(QCPGraph::lsLine);
    this->graph(1)->setLineStyle(QCPGraph::lsLine);

    this->graph(0)->setAntialiased(true);
    this->graph(1)->setAntialiased(true);

    this->legend->setVisible(true);
    this->graph(0)->setName("Asks");
    this->graph(1)->setName("Bids");

    this->graph(0)->setData(priceAsk, quantityAsk);
    this->graph(1)->setData(priceBid, quantityBid);
}