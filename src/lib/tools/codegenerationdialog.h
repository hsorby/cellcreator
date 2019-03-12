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
    void save();

private:
    Ui::CodeGenerationDialog *ui;
};

#endif // CODEGENERATIONDIALOG_H
