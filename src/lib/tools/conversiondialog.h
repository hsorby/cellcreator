#pragma once

#include <QtWidgets>

namespace Ui {
class ConversionDialog;
}

class ConversionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConversionDialog(QWidget *parent=nullptr);
    ConversionDialog(const QString& fileName, QWidget *parent=nullptr);
    ~ConversionDialog();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void okButtonClicked();
    void cancelButtonClicked();

    // Drops
    void performConversion(const QMimeData *mimeData);

    //Buttons
    void chooseOutputFileClicked();
    void saveFileClicked();

private:
    void makeConnections();
    void writeSettings();
    void readSettings();

private:
    Ui::ConversionDialog *ui;
};
