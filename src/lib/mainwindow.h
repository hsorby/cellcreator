#pragma once

#include <QtWidgets>

#include <libcellml>

namespace Ui {
class MainWindow;
}

class DocumentWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    // Actions
    void aboutActionTriggered();
    void openActionTriggered();

    void updateRecentFileActions();
    void openRecentFile();

    void updateToolsMenu();
    void showConversionDialog();
    void showCodeGenerationDialog();

    void convertFileRequestTriggered(const QString& fileName);
    void codeGenerationFileRequestTriggered(libcellml::GeneratorProfile::Profile profile, const QString& fileName);
    void loadNewWindow(const QString& text);

private:
    enum { MaxRecentFiles = 5 };

    void initialise();
    void makeConnections();
    void writeSettings();
    void readSettings();

    bool openFile(const QString &fileName);
    bool loadFile(const QString &fileName);
    DocumentWindow *createMdiChild();
    QMdiSubWindow *findMdiChild(const QString &fileName) const;

    static bool hasRecentFiles();
    void prependToRecentFiles(const QString &fileName);
    void setRecentFilesVisible(bool visible);
    void updateWindowMenu();
    DocumentWindow *activeMdiChild() const;

private:
    Ui::MainWindow *ui;

    QAction *recentFileActs[MaxRecentFiles];
};
