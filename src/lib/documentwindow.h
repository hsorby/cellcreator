
#pragma once

#include <QWidget>

class DocumentWidget;

class DocumentWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DocumentWindow(QWidget *parent = nullptr);
    ~DocumentWindow();

    bool loadFile(const QString& fileName);
    QString currentFile() { return curFile; }
    QString userFriendlyCurrentFile();

private:
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

private:
    DocumentWidget *widget;
    QString curFile;
    bool isUntitled;
};
