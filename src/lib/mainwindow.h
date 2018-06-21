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

private slots:
    void aboutActionTriggered();
    void performConversion(const QMimeData *mimeData);
    void chooseOutputFileClicked();
    void saveFileClicked();

private:
    void makeConnections();

private:
    Ui::MainWindow *ui;
    void writeToFile(const QString &fileName, const QString &content);
};
