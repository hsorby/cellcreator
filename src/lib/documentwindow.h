
#pragma once

#include <QWidget>

#include <libcellml>

class DocumentWidget;

class DocumentWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DocumentWindow(QWidget *parent = nullptr);
    ~DocumentWindow();

    bool loadFile(const QString& fileName);
    bool loadText(const QString& text);
    QString currentFile() { return curFile; }
    QString userFriendlyCurrentFile();

signals:
    void convertFileRequested(const QString& fileName);
    void codeGenerationRequested(libcellml::GeneratorProfile::Profile profile, const QString& fileName);

private slots:
    void conversionRequestTriggered();
    void codeGenerationRequestTriggered(libcellml::GeneratorProfile::Profile profile);

private:
    void makeConnections();
    void setCurrentFile(const QString &fileName);

private:
    DocumentWidget *widget;
    QString curFile;
    bool isUntitled;
};
