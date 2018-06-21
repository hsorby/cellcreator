#pragma once

#include <QtWidgets>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = 0);
    ~AboutDialog();

private slots:
    void okButtonClicked();

private:
    void makeConnections();

private:
    Ui::AboutDialog *ui;
};
