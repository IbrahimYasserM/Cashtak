#include "stdafx.h"
#include "Cashatak.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Cashatak w;
    w.show();
    return a.exec();
}