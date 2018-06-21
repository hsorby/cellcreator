#pragma once

#include <QtWidgets>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void aboutActionTriggered();
    void performConversion(const QMimeData *mimeData);
    void chooseOutputFileClicked();
    void saveFileClicked();

private:
    void makeConnections();
    void writeSettings();
    void readSettings();

private:
    Ui::MainWindow *ui;
};
