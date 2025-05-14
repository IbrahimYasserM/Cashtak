#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CashatakMainWindow.h"

class CashatakMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    CashatakMainWindow(QWidget *parent = nullptr);
    ~CashatakMainWindow();

private:
    Ui::CashatakClass ui;
};
