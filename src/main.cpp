#include "stdafx.h"
#include "ui/windows/CashatakMainWindow.h"
#include <QtWidgets/QApplication>
#include <QFile>
#include <QTextStream>
#include "Database.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Load and apply the global stylesheet
    QFile styleFile(":/style.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream styleStream(&styleFile);
        a.setStyleSheet(styleStream.readAll());
        styleFile.close();
    } else {
        qWarning() << "Failed to load stylesheet:" << styleFile.errorString();
    }

    CashatakMainWindow w;
    w.show();
    return a.exec();
	Database::cleanUp();
}