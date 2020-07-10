#include "UI\StonksMainWindow.h"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    auto model = OrderBookTableModel::getRandomInstance();
    model->addAsk(70, 100000);

    QApplication a(argc, argv);
    StonksMainWindow mainWindow(model);

    mainWindow.show();
    return a.exec();
}
