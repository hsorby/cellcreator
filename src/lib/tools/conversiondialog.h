#pragma once

#include <QtWidgets>

namespace Ui {
class ConversionDialog;
}

class ConversionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConversionDialog(QWidget *parent = nullptr);
    ~ConversionDialog();

private slots:
    void okButtonClicked();

    // Drops
    void performConversion(const QMimeData *mimeData);

    //Buttons
    void chooseOutputFileClicked();
    void saveFileClicked();

private:
    void makeConnections();

private:
    Ui::ConversionDialog *ui;
};
