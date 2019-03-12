
#include "conversiondialog.h"
#include "ui_conversiondialog.h"

#include "manipulate.h"

ConversionDialog::ConversionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConversionDialog)
{
    ui->setupUi(this);
    readSettings();
    makeConnections();
}

ConversionDialog::ConversionDialog(const QString& fileName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConversionDialog)
{
    ui->setupUi(this);
    readSettings();
    makeConnections();
    QMimeData *mimeData = setFileContents(fileName);
    performConversion(mimeData);
}

ConversionDialog::~ConversionDialog()
{
    delete ui;
}

void ConversionDialog::makeConnections()
{
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &ConversionDialog::okButtonClicked);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &ConversionDialog::cancelButtonClicked);
//    connect(ui->pushButtonOk, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
    connect(ui->labelDropTarget, &DropArea::dropped, this, &ConversionDialog::performConversion);
    connect(ui->pushButtonOutputFileXsltTransformed, &QPushButton::clicked, this, &ConversionDialog::chooseOutputFileClicked);
    connect(ui->pushButtonOutputFileLibCellMLPrinted, &QPushButton::clicked, this, &ConversionDialog::chooseOutputFileClicked);
    connect(ui->pushButtonOutputSaveXsltTransformed, &QPushButton::clicked, this, &ConversionDialog::saveFileClicked);
    connect(ui->pushButtonOutputSaveLibCellMLPrinted, &QPushButton::clicked, this, &ConversionDialog::saveFileClicked);
}

void ConversionDialog::okButtonClicked()
{
    close();
}

void ConversionDialog::cancelButtonClicked()
{
    close();
}

void ConversionDialog::chooseOutputFileClicked()
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

void ConversionDialog::saveFileClicked()
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

QMimeData *ConversionDialog::setFileContents(const QString& fileName)
{
    QMimeData *mimeData = nullptr;
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return mimeData;
    }

    mimeData = new QMimeData;
    mimeData->setText(fileName);

    return mimeData;
}

void ConversionDialog::performConversion(const QMimeData *mimeData)
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

void ConversionDialog::readSettings()
{
     QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
     settings.beginGroup("UserInterface");
     settings.beginGroup("Xslt");
     ui->lineEditOutputFileXsltTransformed->setText(settings.value("fileName", QString("")).toString());
     settings.endGroup();
     settings.beginGroup("Printed");
     ui->lineEditOutputFileLibCellMLPrinted->setText(settings.value("fileName", QString("")).toString());
     settings.endGroup();
     settings.endGroup();
}

void ConversionDialog::writeSettings()
{
     QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
     settings.beginGroup("UserInterface");
     settings.beginGroup("Xslt");
     settings.setValue("fileName", ui->lineEditOutputFileXsltTransformed->text());
     settings.endGroup();
     settings.beginGroup("Printed");
     settings.setValue("fileName", ui->lineEditOutputFileLibCellMLPrinted->text());
     settings.endGroup();
     settings.endGroup();
}

void ConversionDialog::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}
