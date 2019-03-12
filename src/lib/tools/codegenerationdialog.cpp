#include "codegenerationdialog.h"
#include "ui_codegenerationdialog.h"

#include <QtWidgets>

#include "manipulate.h"
#include "utilities.h"

CodeGenerationDialog::CodeGenerationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CodeGenerationDialog)
{
    ui->setupUi(this);
}

CodeGenerationDialog::CodeGenerationDialog(const QString& fileName, QWidget *parent)
  : QDialog(parent)
  , ui(new Ui::CodeGenerationDialog)
{
    ui->setupUi(this);
    QMimeData *mimeData = setMimeDataText(fileName, true);
    QString out = "";
    QString msg = "";
    QString cellmlModel = "";
    bool cellML1File = isCellML1(mimeData->text(), &out, &msg);
    if (cellML1File) {
        bool success = xsltTransfrom(mimeData->text(), &out, &msg);
        if (success) {
            cellmlModel = out;
        }
    } else {
        bool cellML2File = isCellML2(mimeData->text(), &out, &msg);
        if (cellML2File) {
            QFile file(mimeData->text());
            if (file.open(QFile::ReadOnly | QFile::Text)) {
                QTextStream in(&file);
                cellmlModel = in.readAll();
            }
            file.close();
        }
    }

    if (!cellmlModel.isEmpty()) {
        std::string generatedCode = generateCode(cellmlModel.toStdString());
        qDebug() << generatedCode.c_str();
        ui->plainTextEdit->setPlainText(generatedCode.c_str());
    }
}

CodeGenerationDialog::~CodeGenerationDialog()
{
    delete ui;
}

void CodeGenerationDialog::save()
{
    qDebug() << "saving";
}
