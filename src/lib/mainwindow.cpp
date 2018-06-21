
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <sstream>

#include <QXmlQuery>

#include <libcellml>

#include "aboutdialog.h"
#include "manipulate.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
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

void MainWindow::performConversion(const QMimeData *mimeData)
{
    if (mimeData == nullptr) {
        return;
    }

    if (!mimeData->hasText()) {
        return;
    }

    QString out = "";
    QString msg = "";
    bool success = xsltTransfrom(mimeData->text(), &out, &msg);

    ui->textEditReport->clear();
    ui->plainTextEditLibCellMLPrinted->clear();
    ui->plainTextEditXslt->clear();
    if (success) {
        ui->labelDropTarget->setText(mimeData->text());

        std::string parseResponse = parseText(out.toStdString());
        ui->textEditReport->insertHtml(parseResponse.c_str());

        ui->plainTextEditXslt->insertPlainText(out);
        ui->plainTextEditXslt->moveCursor(QTextCursor::End);

        std::string printedForm = libCellMLPrintModel(out.toStdString());
        ui->plainTextEditLibCellMLPrinted->insertPlainText(printedForm.c_str());
        ui->plainTextEditLibCellMLPrinted->moveCursor(QTextCursor::End);
    } else if (msg.length() > 0) {
        ui->textEditReport->insertHtml(msg);
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
