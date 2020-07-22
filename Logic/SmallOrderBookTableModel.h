#pragma once

#include <QAbstractTableModel>

#include "Order.h"
#include "OrderBook.h"

class SmallOrderBookTableModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	static const int MAX_VISIBLE_ASKS_AMOUNT = 10;
	static const int MAX_VISIBLE_ORDERS_AMOUNT = MAX_VISIBLE_ASKS_AMOUNT * 2;

	QList<Order*> rows;
	SmallOrderBookTableModel(QObject* parent = nullptr);

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	Qt::ItemFlags flags(const QModelIndex& index) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	bool setData(const QModelIndex& index, const QVariant& value, int role) override;

	// Изменение данных таблицы
	void changeData(OrderBook* orderBook);
	void updateTable();

private:
	QStringList headers;

};