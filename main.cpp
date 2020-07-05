#include "UI\StonksMainWindow.h"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StonksMainWindow mainWindow;
    mainWindow.placeMarketDepthGraph();
    mainWindow.show();
    return a.exec();
}
