#include "stdafx.h"
#include "ui/windows/CashatakMainWindow.h"
#include <QtWidgets/QApplication>
#include <QFile>
#include <QTextStream>
#include "core/Database.h"

void cleanup() {
    Database::cleanUp();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Connect cleanup to application quit
    QObject::connect(&a, &QApplication::aboutToQuit, cleanup);

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
}