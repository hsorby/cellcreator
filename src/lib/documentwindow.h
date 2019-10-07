
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

signals:
    void convertFileRequested(const QString& fileName);
    void codeGenerationRequested(const QString& fileName);

private:
    void makeConnections();
    void setCurrentFile(const QString &fileName);
    void conversionRequestTriggered();
    void codeGenerationRequestTriggered();

private:
    DocumentWidget *widget;
    QString curFile;
    bool isUntitled;
};
