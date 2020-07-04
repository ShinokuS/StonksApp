#pragma once

#include <QtWidgets/QMainWindow>

#include "ui_StonksApp.h"

class StonksApp : public QMainWindow
{
    Q_OBJECT

public:
    StonksApp(QWidget *parent = Q_NULLPTR);

private:
    Ui::StonksAppClass ui;
};
