#include "OrderBookTableModel.h"

#include <QSize>
#include <QBrush>

OrderBookTableModel::OrderBookTableModel(QObject* parent)
    : QAbstractTableModel(parent)
{
    centerIndex = 0;
    headers << "Price" << "Quantity";
}

QVariant OrderBookTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section < headers.size()) {
        return headers.at(section);
    }
    return {};
}

int OrderBookTableModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;
        return rows.size();
}

int OrderBookTableModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;
        return headers.size();
}

QVariant OrderBookTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    auto row = rows.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        switch (index.column()) {
        case 0: return row->price;
        case 1: return row->quantity;
        }
    case Qt::TextAlignmentRole:
        return Qt::AlignCenter;
    case Qt::BackgroundRole:
        if (row->isAsk) {
            return QBrush(0xff6161);
        }
        else return QBrush(0x7ce670);
    }
    return QVariant();
}

int randomBetween(int begin, int end)
{
    return begin + rand() % (end - begin);
}

OrderBookTableModel* OrderBookTableModel::getRandomInstance(unsigned int seed)
{
    // настройки рандома
    srand(seed);
    int nBids = randomBetween(20, 100);
    int nAsks = randomBetween(20, 100);
    long long minBidPrice = 100;
    long long minAskPrice = 10000;
    long long maxAskPrice = 20000;
    int maxQuantityInOrder = 10000;

    auto randomOrderBook = new OrderBookTableModel;

    for (int i = 0; i < nBids; i++) {
        qreal price = randomBetween(minBidPrice, minAskPrice);
        price /= 100; // перевод из копеек
        qreal quantity = randomBetween(1, maxQuantityInOrder);
        randomOrderBook->addBid(price, quantity);
    }

    for (int i = 0; i < nAsks; i++) {
        qreal price = randomBetween(minAskPrice, maxAskPrice);
        price /= 100; // перевод из копеек
        qreal quantity = randomBetween(1, maxQuantityInOrder);
        randomOrderBook->addAsk(price, quantity);
    }

    return randomOrderBook;
}

void OrderBookTableModel::updateTable()
{
    emit layoutAboutToBeChanged();
    emit layoutChanged();
}

int OrderBookTableModel::returnCenterIndex()
{
    return OrderBookTableModel::centerIndex;
}

bool OrderBookTableModel::setData(const QModelIndex& index, const QVariant&, int role)
{
    if (!index.isValid()) return false;
    return true;
}

Qt::ItemFlags OrderBookTableModel::flags(const QModelIndex& index) const
{
    return QAbstractTableModel::flags(index) | Qt::ItemIsUserCheckable;
}

void OrderBookTableModel::addBid(qreal price, qreal quantity, time_t time)
{
    // Если добавляемый ордер вызывает сделки, проводим их.
    auto newBid = new Order { price, quantity, false, time };
    makeDealsIfNeededFor(newBid);

    // Если ордер не исполнился полностью, то добавляем его в ордербук.
    if (newBid->quantity > 0) {
        addBidToList(newBid);
    }
}

void OrderBookTableModel::addAsk(qreal price, qreal quantity, time_t time)
{
    // Если добавляемый ордер вызывает сделки, проводим их.
    auto newAsk = new Order { price, quantity, true, time };
    makeDealsIfNeededFor(newAsk);

    // Если ордер не исполнился полностью, то добавляем его в ордербук.
    if (newAsk->quantity > 0) {
        addAskToList(newAsk);
    }
}

void OrderBookTableModel::makeDealsIfNeededFor(Order* newOrder)
{
    // Обеспечиваю обобщение под бид/аск
    int endIndex, endIndexStep, indexStep, centerIndexStep;

    if (newOrder->isAsk) {
        endIndex = rows.size();
        endIndexStep = -1;
        indexStep = 0;
        centerIndexStep = 0;
    }
    else {
        endIndex = -1;
        endIndexStep = 0;
        indexStep = -1;
        centerIndexStep = -1;
    }
    
    int rowIndex = centerIndex;
    if (! newOrder->isAsk) {
        rowIndex--;
    }

    // Теперь само проведение сделок, начиная с самых выгодных висящих ордеров:

    for (/*  */; newOrder->quantity > 0 && rowIndex != endIndex; rowIndex += indexStep)
    {
        if (! canMakeDealBetween(newOrder, rows[rowIndex])) {
            break; // приемлемые сделки закончились
        }

        // Старые ордеры поглощаются новым
        if (rows[rowIndex]->quantity <= newOrder->quantity) {
            newOrder->quantity -= rows[rowIndex]->quantity;
            // <------ ЗДЕСЬ БУДЕТ СОХРАНЕНИЕ СДЕЛКИ В СПИСОК СДЕЛОК
            delete rows[rowIndex];
            rows.removeAt(rowIndex);
            endIndex += endIndexStep;
            centerIndex += centerIndexStep;
        }
        // Последний, вероятно, поглотится не полностью:
        else {
            // <------ ЗДЕСЬ БУДЕТ СОХРАНЕНИЕ СДЕЛКИ В СПИСОК СДЕЛОК
            rows[rowIndex]->quantity -= newOrder->quantity;
            newOrder->quantity = 0;
        }
    }
}

bool OrderBookTableModel::canMakeDealBetween(Order* one, Order* other)
{
    if (one->isAsk == other->isAsk) {
        return false;
    }
    else if (one->isAsk) {
        return one->price <= other->price;
    }
    else {
        return one->price >= other->price;
    }
}

// НИЖЕ, ОБА МЕТОДА ВСТАВКИ ТРАТЯТ ПО O(n) НА ВСТАВКУ!
// НАДЕЮСЬ, ЭТО ВРЕМЕННОЕ РЕШЕНИЕ!

void OrderBookTableModel::addBidToList(Order* newBid)
{
    Order reference = { newBid->price, 0, false }; // костыль из-за интерфейса STL

    // бинарный поиск позиции с ценой не меньше указанной
    auto iter = std::lower_bound(rows.begin(), rows.end(), reference,
        [reference](Order* element, const Order reference) { return element->price > reference.price; });

    // если это будет новая самая дешёвая позиция, то создаём её в конце списка
    if (iter == rows.end()) {
        rows.append(newBid);
    }
    // если не самая, но конкретно такой цены нет, то создаём её в нужном месте
    else if ((*iter)->price < newBid->price) {
        rows.insert(iter, newBid);
    }
    // если позиция есть, то приплюсовываем туда количество
    else {
        (*iter)->quantity += newBid->quantity;
    }
}

void OrderBookTableModel::addAskToList(Order* newAsk)
{
    Order reference = { newAsk->price, 0, true }; // костыль из-за интерфейса STL

    // бинарный поиск позиции с ценой не меньше указанной
    auto iter = std::lower_bound(rows.begin(), rows.end(), reference,
        [reference](Order* element, const Order reference) { return element->price > reference.price; });

    // если позиции с такой ценой нет, то создаём её в нужном месте
    if ((*iter)->price < newAsk->price) {
        rows.insert(iter, newAsk);
        centerIndex++;
    }
    // если позиция есть, то приплюсовываем туда количество
    else {
        (*iter)->quantity += newAsk->quantity;
    }
}