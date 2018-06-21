
#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include <libcellml>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->labelLibCellMLVersion->setText(libcellml::versionString().c_str());
    makeConnections();
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::makeConnections()
{
    connect(ui->pushButtonOk, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
}

void AboutDialog::okButtonClicked()
{
    close();
}
