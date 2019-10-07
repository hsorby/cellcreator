#include "documentwidget.h"
#include "ui_documentwidget.h"

#include <QtWidgets>

const QString C_CODE_ACTION_TEXT = "C Code";
const QString PYTHON_CODE_ACTION_TEXT = "Python Code";


DocumentWidget::DocumentWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DocumentWidget)
{
    ui->setupUi(this);
    ui->textEdit->setContextMenuPolicy(Qt::CustomContextMenu);
    makeConnections();
}

DocumentWidget::~DocumentWidget()
{
    delete ui;
}

void DocumentWidget::makeConnections()
{
    connect(ui->textEdit, &DocumentWidget::customContextMenuRequested, this, &DocumentWidget::customMenuRequested);
}

void DocumentWidget::setPlainText(const QString& text)
{
    ui->textEdit->setPlainText(text);
}

QMenu *DocumentWidget::createStandardContextMenu()
{
    return ui->textEdit->createStandardContextMenu();
}

void DocumentWidget::customMenuRequested(const QPoint& pos)
{
    QMenu *contextMenu = ui->textEdit->createStandardContextMenu();
    contextMenu->addSeparator();
    QAction *convertAction = new QAction("Convert to CellML 2.0");
    connect(convertAction, &QAction::triggered, this, &DocumentWidget::conversionRequested);

    QMenu *codeGenerationMenu = new QMenu("Generate Code");
    QAction *codeGenerationPythonAction = new QAction(C_CODE_ACTION_TEXT);
    connect(codeGenerationPythonAction, &QAction::triggered, this, &DocumentWidget::codeGenerationTriggered);
    QAction *codeGenerationCAction = new QAction(PYTHON_CODE_ACTION_TEXT);
    connect(codeGenerationCAction, &QAction::triggered, this, &DocumentWidget::codeGenerationTriggered);

    codeGenerationMenu->addAction(codeGenerationCAction);
    codeGenerationMenu->addAction(codeGenerationPythonAction);

    contextMenu->addAction(convertAction);
    contextMenu->addMenu(codeGenerationMenu);
    contextMenu->popup(mapToGlobal(pos));
}

void DocumentWidget::codeGenerationTriggered()
{
    libcellml::GeneratorProfile::Profile profile = libcellml::GeneratorProfile::Profile::C;
    if (const QAction *action = qobject_cast<const QAction *>(sender())) {
        qDebug() << "I know this sender" << action->text();
        if (action->text() == C_CODE_ACTION_TEXT) {
            profile = libcellml::GeneratorProfile::Profile::C;
        } else if (action->text() == PYTHON_CODE_ACTION_TEXT) {
            profile = libcellml::GeneratorProfile::Profile::PYTHON;
        }
    }
    emit codeGenerationRequested(profile);
}
