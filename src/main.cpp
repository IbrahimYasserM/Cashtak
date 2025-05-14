#include "stdafx.h"
#include "ui/windows/CashatakMainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CashatakMainWindow w;
    w.show();
    return a.exec();
}