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
    ui->tabWidget->clear(); // Part of setting up the ui.
    makeConnections();
    updateUi();
}

CodeGenerationDialog::CodeGenerationDialog(libcellml::GeneratorProfile::Profile profile, const QString& fileName, QWidget *parent)
  : QDialog(parent)
  , ui(new Ui::CodeGenerationDialog)
{
    ui->setupUi(this);
    ui->tabWidget->clear(); // Part of setting up the ui.
    generateCode(profile, fileName);
    makeConnections();
    updateUi();
}

CodeGenerationDialog::~CodeGenerationDialog()
{
    delete ui;
}

void CodeGenerationDialog::makeConnections()
{
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(updateUi()));
    connect(ui->pushButtonOpen, SIGNAL(clicked()), this, SLOT(openButtonClicked()));
    connect(ui->pushButtonGenerate, SIGNAL(clicked()), this, SLOT(generateButtonClicked()));
    connect(ui->pushButtonLoad, SIGNAL(clicked()), this, SLOT(loadButtonClicked()));
    connect(ui->pushButtonLoadAll, SIGNAL(clicked()), this, SLOT(loadAllButtonClicked()));
    connect(ui->pushButtonClose, SIGNAL(clicked()), this, SLOT(closeButtonClicked()));
}

void CodeGenerationDialog::openFile(const QString& fileName)
{
    auto content = fileContents(fileName);
    createTab(strippedName(fileName), content);
    auto plainTextEdit = ui->tabWidget->currentWidget();
    plainTextEdit->setToolTip(fileName);
}

void CodeGenerationDialog::updateUi()
{
    bool haveFiles = ui->tabWidget->count() > 0;
    auto activeWidget = ui->tabWidget->currentWidget();
    bool activeTabIsGenerated = activeWidget ? activeWidget->toolTip().isEmpty() : false;

    ui->stackedWidget->setCurrentIndex(haveFiles ? 1 : 0);
    ui->pushButtonGenerate->setEnabled(!activeTabIsGenerated && haveFiles);
    ui->pushButtonLoad->setEnabled(activeTabIsGenerated);
    ui->pushButtonLoadAll->setEnabled(activeTabIsGenerated);

    ui->pushButtonClose->setEnabled(haveFiles);
}

void CodeGenerationDialog::closeButtonClicked()
{
    ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
    updateUi();
}

void CodeGenerationDialog::generateButtonClicked()
{
    auto activeWidget = ui->tabWidget->currentWidget();
    generateCode(libcellml::GeneratorProfile::Profile::C, activeWidget->toolTip());
}

void CodeGenerationDialog::createTab(const QString& title, const QString& content)
{
    auto plainTextEdit = new QPlainTextEdit();
    plainTextEdit->setPlainText(content);
    ui->tabWidget->addTab(plainTextEdit, title);
}

void CodeGenerationDialog::loadButtonClicked()
{
    qDebug() << "return active generated code tab";
}

void CodeGenerationDialog::loadAllButtonClicked()
{
    qDebug() << "return all generated code tabs";
}

void CodeGenerationDialog::openButtonClicked()
{
    const QString settingsPath = "tools/CodeGeneration";
    QString fileName =  QFileDialog::getOpenFileName(this, "Open Document", previousLocation(settingsPath),
                                                     "CellML files (*.cellml *.xml) ;; All files (*.*)");
    if( !fileName.isNull() ) {
        openFile(fileName);
        setPreviousLocation(settingsPath, fileDirectory(fileName));
        updateUi();
    }
}

void CodeGenerationDialog::generateCode(libcellml::GeneratorProfile::Profile profile, const QString& fileName)
{
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
            cellmlModel = fileContents(mimeData->text());
        }
    }

    if (!cellmlModel.isEmpty()) {
        GeneratedCode generatedCode = ::generateCode(profile, cellmlModel.toStdString());
        switch (generatedCode.profile) {
        case libcellml::GeneratorProfile::Profile::C:
            createTab(strippedName(fileName) + " C Impl.", generatedCode.implementationCode.c_str());
            createTab(strippedName(fileName) + " C Inter.", generatedCode.interfaceCode.c_str());
            break;
        case libcellml::GeneratorProfile::Profile::PYTHON:
            createTab(strippedName(fileName) + " Python", generatedCode.implementationCode.c_str());
            break;
        }

    }
}
