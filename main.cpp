#include "UI\StonksApp.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StonksApp w;
    w.show();
    return a.exec();
}
