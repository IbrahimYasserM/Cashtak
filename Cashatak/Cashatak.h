#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Cashatak.h"

class Cashatak : public QMainWindow
{
    Q_OBJECT

public:
    Cashatak(QWidget *parent = nullptr);
    ~Cashatak();

private:
    Ui::CashatakClass ui;
};
