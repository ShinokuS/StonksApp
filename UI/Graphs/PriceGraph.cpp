#include "PriceGraph.h"

PriceGraph::PriceGraph(QVector <double> time, QVector <double> price)
{
    this->setInteraction(QCP::iRangeZoom, true);   // Включаем взаимодействие удаления/приближения
    this->setInteraction(QCP::iRangeDrag, true);  // Включаем взаимодействие перетаскивания графика
    this->axisRect()->setRangeDrag(Qt::Horizontal);   // Включаем перетаскивание только по горизонтальной оси
    this->axisRect()->setRangeZoom(Qt::Horizontal);   // Включаем удаление/приближение только по горизонтальной оси
    this->xAxis->setTickLabelType(QCPAxis::ltDateTime);   // Подпись координат по Оси X в качестве Даты и Времени
    this->xAxis->setDateTimeFormat("hh:mm");  // Устанавливаем формат даты и времени

    // Ставим шрифты на ось
    this->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    this->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
    
    // Отключаем автоматическое масштабирование тиков по Оси X и ставим шаг 2 часа
    this->xAxis->setAutoTickStep(false);
    this->xAxis->setTickStep(7200);
    
    // Делаем видимой всю часть графика
    /* Делаем видимыми оси X и Y по верхней и правой границам графика,
       но отключаем на них тики и подписи координат
     * */
    this->xAxis2->setVisible(true);
    this->yAxis2->setVisible(true);
    this->xAxis2->setTicks(false);
    this->yAxis2->setTicks(false);
    this->xAxis2->setTickLabels(false);
    this->yAxis2->setTickLabels(false);

    this->yAxis->setTickLabelColor(QColor(0xff6161)); // Красный цвет подписей тиков по Оси Y

    // Инициализируем график и привязываем его к Осям
    QCPGraph *graphic = new QCPGraph(this->xAxis, this->yAxis);
    this->addPlottable(graphic);  // Устанавливаем график на полотно
    graphic->setPen(QPen(QColor(0xff6161),1.5)); // Устанавливаем цвет графика
    graphic->setAntialiased(true);         // Отключаем сглаживание, по умолчанию включено
    graphic->setLineStyle(QCPGraph::lsLine); // График в виде линии
    graphic->setData(time, price); // Устанавливаем данные
    this->rescaleAxes();
}
