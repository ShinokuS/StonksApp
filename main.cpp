#include "UI\StonksMainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    auto deals = new Deals();
    auto model = OrderBookTableModel::getRandomInstance();

    QApplication a(argc, argv);
    StonksMainWindow mainWindow(model, deals);
    mainWindow.show();

    return a.exec();
}
