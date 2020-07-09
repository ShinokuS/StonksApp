#include "UI\StonksMainWindow.h"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    auto model = OrderBookTableModel::getRandomInstance();

    QApplication a(argc, argv);
    StonksMainWindow mainWindow(model);

    mainWindow.show();
    return a.exec();
}
