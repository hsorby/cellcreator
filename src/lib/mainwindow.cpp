
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <libcellml>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    makeConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::makeConnections()
{
    connect(ui->pushButtonVersion, SIGNAL(clicked()), this, SLOT(versionButtonClicked()));
}

void MainWindow::versionButtonClicked()
{
    ui->labelVersion->setText(libcellml::versionString().c_str());
}
