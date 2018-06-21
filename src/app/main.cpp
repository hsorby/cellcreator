#include <QApplication>

#include "mainwindow.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("CellML");
    QCoreApplication::setOrganizationDomain("cellml.org");
    QCoreApplication::setApplicationName("CellCreator");

    MainWindow mw;
    mw.show();

    return app.exec();
}

