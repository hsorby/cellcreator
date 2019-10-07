#include <QApplication>

#include "mainwindow.h"
#include "manipulate.h"
#include "utilities.h"

int main(int argc, char **argv)
{
    QCoreApplication *app;
    if (argc > 1) {
        app = new QCoreApplication(argc, argv);
    } else {
        app = new QApplication(argc, argv);
    }

    QCoreApplication::setOrganizationName("CellML");
    QCoreApplication::setOrganizationDomain("cellml.org");
    QCoreApplication::setApplicationName("CellCreator");

    QCommandLineParser parser;
    parser.setApplicationDescription("Cell Creator");
    parser.addHelpOption();
    parser.addVersionOption();

    // An option for setting the output of the printed CellML 2.0 file.
    QCommandLineOption outputOption(QStringList() << "o" << "output", "output CellML 2.0 file printed by libCellML.", "file");
    parser.addOption(outputOption);
    QCommandLineOption xsltOption(QStringList() << "t" << "xslt", "output CellML 2.0 file transformed by xslt.", "file");
    parser.addOption(xsltOption);
    parser.addPositionalArgument("source", "Source CellML 1.1 (or 1.0) file to transform.");

    // Process the actual command line arguments given by the user
    parser.process(*app);

    initialiseResources();
    registerMetatypes();
    int result = -1;
    MainWindow *mw = nullptr;
    if (argc > 1) {
        if (parser.isSet(outputOption) || parser.isSet(xsltOption)) {
            const QStringList args = parser.positionalArguments();
            const QString source = args.isEmpty() ? QString() : args.first();
            QFileInfo fi(source);
            if (fi.exists()) {
                QString out = "";
                QString msg = "";
                bool success = xsltTransfrom(source, &out, &msg);
                if (success) {
                    if (parser.isSet(xsltOption)) {
                        writeToFile(parser.value(xsltOption), out);
                    }
                    if (parser.isSet(outputOption)) {
                        std::string printedForm = libCellMLPrintModel(out.toStdString());
                        writeToFile(parser.value(outputOption), printedForm.c_str());
                    }
                    result = 0;
                } else {
                    qDebug() << msg;
                }
            } else {
                qDebug() << "Source file is not set or does not exist.";
            }
        } else {
            qDebug() << "Neither CellML output option nor xslt option is set.";
            result = -2;
        }
    } else {
        mw = new MainWindow();
        mw->show();
        result = app->exec();
    }
    if (mw != nullptr) {
        delete mw;
    }
    delete app;

    return result;
}

