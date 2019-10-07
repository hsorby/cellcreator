#include "documentwidget.h"
#include "ui_documentwidget.h"

#include <QtWidgets>

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
    QAction *codeGenerationPythonAction = new QAction("C Code");
    connect(codeGenerationPythonAction, &QAction::triggered, this, &DocumentWidget::codeGenerationRequested);
    QAction *codeGenerationCAction = new QAction("Python Code");
    connect(codeGenerationCAction, &QAction::triggered, this, &DocumentWidget::codeGenerationRequested);

    codeGenerationMenu->addAction(codeGenerationCAction);
    codeGenerationMenu->addAction(codeGenerationPythonAction);

    contextMenu->addAction(convertAction);
    contextMenu->addMenu(codeGenerationMenu);
    contextMenu->popup(mapToGlobal(pos));
}
