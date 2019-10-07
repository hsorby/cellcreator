#ifndef CODEGENERATIONDIALOG_H
#define CODEGENERATIONDIALOG_H

#include <QDialog>

namespace Ui {
class CodeGenerationDialog;
}

class CodeGenerationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CodeGenerationDialog(QWidget *parent = nullptr);
    CodeGenerationDialog(const QString& fileName, QWidget *parent = nullptr);
    ~CodeGenerationDialog();

private slots:
    void closeButtonClicked();
    void generateButtonClicked();
    void loadButtonClicked();
    void loadAllButtonClicked();
    void openButtonClicked();
    void updateUi();

private:
    void createTab(const QString& title, const QString& content);
    void generateCode(const QString& fileName);
    void makeConnections();
    void openFile(const QString& fileName);

private:
    Ui::CodeGenerationDialog *ui;
};

#endif // CODEGENERATIONDIALOG_H
