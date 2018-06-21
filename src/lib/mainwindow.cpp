
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <sstream>

#include <QXmlQuery>

#include <libcellml>

#include "aboutdialog.h"
#include "messagehandler.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    Q_INIT_RESOURCE(resources);

    ui->setupUi(this);

    readSettings();
    makeConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::makeConnections()
{
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::aboutActionTriggered);
    connect(ui->labelDropTarget, &DropArea::dropped, this, &MainWindow::performConversion);
    connect(ui->pushButtonOutputFileXsltTransformed, &QPushButton::clicked, this, &MainWindow::chooseOutputFileClicked);
    connect(ui->pushButtonOutputFileLibCellMLPrinted, &QPushButton::clicked, this, &MainWindow::chooseOutputFileClicked);
    connect(ui->pushButtonOutputSaveXsltTransformed, &QPushButton::clicked, this, &MainWindow::saveFileClicked);
    connect(ui->pushButtonOutputSaveLibCellMLPrinted, &QPushButton::clicked, this, &MainWindow::saveFileClicked);
}

void MainWindow::aboutActionTriggered()
{
    AboutDialog dlg(this);
    dlg.exec();
}

void MainWindow::chooseOutputFileClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("CellML 2.0 output file"), "",
            tr("CellML (*.cellml);;All Files (*)"));
    if (sender() == ui->pushButtonOutputFileLibCellMLPrinted || sender() == ui->pushButtonOutputSaveLibCellMLPrinted) {
        ui->lineEditOutputFileLibCellMLPrinted->setText(fileName);
    } else if (sender() == ui->pushButtonOutputFileXsltTransformed || sender() == ui->pushButtonOutputSaveXsltTransformed) {
        ui->lineEditOutputFileXsltTransformed->setText(fileName);
    }
}

void MainWindow::saveFileClicked()
{
    QString fileName = "";
    QString content = "";
    if (sender() == ui->pushButtonOutputSaveLibCellMLPrinted) {
        fileName = ui->lineEditOutputFileLibCellMLPrinted->text();
        content = ui->plainTextEditLibCellMLPrinted->toPlainText();
    } else if (sender() == ui->pushButtonOutputSaveXsltTransformed) {
        fileName = ui->lineEditOutputFileXsltTransformed->text();
        content = ui->plainTextEditXslt->toPlainText();
    }

    if (fileName.isEmpty()) {
        chooseOutputFileClicked();
    }

    writeToFile(fileName, content);
}

void MainWindow::writeToFile(const QString &fileName, const QString &content)
{
    if (fileName.length() > 0) {
        QFile outFile(fileName);
        if (outFile.open(QIODevice::WriteOnly)) {
            outFile.write(content.toUtf8().constData());
            outFile.close();
        }
    }
}

void MainWindow::performConversion(const QMimeData *mimeData)
{
    if (mimeData == nullptr) {
        return;
    }

    if (!mimeData->hasText()) {
        return;
    }

    MessageHandler messageHandler;

    bool success = false;
    QFile xslt(":/xslt/cellml1to2.xsl");
    QString out = "";
    if (xslt.open(QIODevice::ReadOnly)) {
        QXmlQuery query(QXmlQuery::XSLT20);
        query.setMessageHandler(&messageHandler);
        success = query.setFocus(QUrl(mimeData->text()));
        if (success) {
            query.setQuery(xslt.readAll());
            success = query.evaluateTo(&out);
        }
        xslt.close();
    }

    ui->textEditReport->clear();
    ui->plainTextEditLibCellMLPrinted->clear();
    ui->plainTextEditXslt->clear();
    if (success) {
        ui->labelDropTarget->setText(mimeData->text());
        libcellml::Parser parser;
        libcellml::ModelPtr model = parser.parseModel(out.toStdString());

        std::ostringstream os;
        os << "<html xmlns='http://www.w3.org/1999/xhtml/'><body>" << std::endl;
        os << "<h1><bold>Parsing errors</bold></h1>" << std::endl;
        if (parser.errorCount() == 0) {
            os << "<ul><li>No parsing errors.</li></ul>" << std::endl;
        } else {
            os << "<ol>" << std::endl;
            for (size_t i = 0; i < parser.errorCount(); ++i) {
                os << "<li>" << parser.getError(i)->getDescription() << "</li>" << std::endl;
            }
            os << "</ol>" << std::endl;
        }
        libcellml::Validator validator;
        validator.validateModel(model);
        os << std::endl;
        os << "<h1><bold>Validation errors</bold></h1>" << std::endl;
        if (validator.errorCount() == 0) {
            os << "<ul><li>No validation erros.</li></ul>" << std::endl;
        } else {
            os << "<ol>" << std::endl;
            for (size_t i = 0; i < validator.errorCount(); ++i) {
                os << "<li>" << validator.getError(i)->getDescription() << "</li>" << std::endl;
            }
            os << "</ol>" << std::endl;
        }
        os << std::endl << std::endl;
        os << "</body></html>" << std::endl;
        ui->textEditReport->insertHtml(os.str().c_str());

        ui->plainTextEditXslt->insertPlainText(out);
        ui->plainTextEditXslt->moveCursor(QTextCursor::End);

        libcellml::Printer printer;
        QString printedForm(printer.printModel(model).c_str());
        ui->plainTextEditLibCellMLPrinted->insertPlainText(printedForm);
        ui->plainTextEditLibCellMLPrinted->moveCursor(QTextCursor::End);
    } else if (messageHandler.statusMessage().length() > 0) {
        ui->textEditReport->insertHtml(messageHandler.statusMessage());
    }
    ui->textEditReport->moveCursor(QTextCursor::End);
}

void MainWindow::writeSettings()
{
    QSettings settings;

    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
    settings.beginGroup("UserInterface");
    settings.beginGroup("Xslt");
    settings.setValue("fileName", ui->lineEditOutputFileXsltTransformed->text());
    settings.endGroup();
    settings.beginGroup("Printed");
    settings.setValue("fileName", ui->lineEditOutputFileLibCellMLPrinted->text());
    settings.endGroup();
    settings.endGroup();
}

void MainWindow::readSettings()
{
    QSettings settings;

    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(400, 400)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    settings.endGroup();
    settings.beginGroup("UserInterface");
    settings.beginGroup("Xslt");
    ui->lineEditOutputFileXsltTransformed->setText(settings.value("fileName", QString("")).toString());
    settings.endGroup();
    settings.beginGroup("Printed");
    ui->lineEditOutputFileLibCellMLPrinted->setText(settings.value("fileName", QString("")).toString());
    settings.endGroup();
    settings.endGroup();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}
