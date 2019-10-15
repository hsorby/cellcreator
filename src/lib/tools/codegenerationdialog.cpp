#include "codegenerationdialog.h"
#include "ui_codegenerationdialog.h"

#include <QtWidgets>

#include "manipulate.h"
#include "utilities.h"

const QString C_CODE_ACTION_TEXT = "C Code";
const QString PYTHON_CODE_ACTION_TEXT = "Python Code";

CodeGenerationDialog::CodeGenerationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CodeGenerationDialog)
{
    setupUi();
    makeConnections();
    updateUi();
}

CodeGenerationDialog::CodeGenerationDialog(libcellml::GeneratorProfile::Profile profile, const QString& fileName, QWidget *parent)
  : QDialog(parent)
  , ui(new Ui::CodeGenerationDialog)
{
    setupUi();
    generateCode(profile, fileName);
    makeConnections();
    updateUi();
}

CodeGenerationDialog::~CodeGenerationDialog()
{
    delete ui;
}

void CodeGenerationDialog::setupUi()
{
    ui->setupUi(this);
    ui->tabWidget->clear(); // Part of setting up the ui.
    QMenu *codeGenerationMenu = new QMenu("Generate Code", this);
    QAction *codeGenerationPythonAction = new QAction(C_CODE_ACTION_TEXT);
    connect(codeGenerationPythonAction, &QAction::triggered, this, &CodeGenerationDialog::codeGenerationTriggered);
    QAction *codeGenerationCAction = new QAction(PYTHON_CODE_ACTION_TEXT);
    connect(codeGenerationCAction, &QAction::triggered, this, &CodeGenerationDialog::codeGenerationTriggered);

    codeGenerationMenu->addAction(codeGenerationCAction);
    codeGenerationMenu->addAction(codeGenerationPythonAction);

    ui->pushButtonGenerate->setMenu(codeGenerationMenu);
}

void CodeGenerationDialog::makeConnections()
{
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(updateUi()));
    connect(ui->pushButtonOpen, SIGNAL(clicked()), this, SLOT(openButtonClicked()));
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

void CodeGenerationDialog::codeGenerationTriggered()
{
    auto activeWidget = ui->tabWidget->currentWidget();
    libcellml::GeneratorProfile::Profile profile = libcellml::GeneratorProfile::Profile::C;
    if (const QAction *action = qobject_cast<const QAction *>(sender())) {
        if (action->text() == C_CODE_ACTION_TEXT) {
            profile = libcellml::GeneratorProfile::Profile::C;
        } else if (action->text() == PYTHON_CODE_ACTION_TEXT) {
            profile = libcellml::GeneratorProfile::Profile::PYTHON;
        }
    }
    generateCode(profile, activeWidget->toolTip());
}

void CodeGenerationDialog::createTab(const QString& title, const QString& content)
{
    auto plainTextEdit = new QPlainTextEdit();
    plainTextEdit->setPlainText(content);
    ui->tabWidget->addTab(plainTextEdit, title);
}

void CodeGenerationDialog::loadButtonClicked()
{
    auto activeWidget = qobject_cast<const QPlainTextEdit *>(ui->tabWidget->currentWidget());
    emit loadText(activeWidget->toPlainText());
}

void CodeGenerationDialog::loadAllButtonClicked()
{
    for (int i = 0; i < ui->tabWidget->count(); ++i) {
        auto widget = qobject_cast<const QPlainTextEdit *>(ui->tabWidget->widget(i));
        if (widget->toolTip().isEmpty()) {
            emit loadText(widget->toPlainText());
        }
    }
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
