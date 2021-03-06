
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <sstream>

#include "aboutdialog.h"
#include "codegenerationdialog.h"
#include "conversiondialog.h"
#include "documentwidget.h"
#include "documentwindow.h"
#include "utilities.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    readSettings();
    initialise();
    makeConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::makeConnections()
{
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::aboutActionTriggered);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openActionTriggered);
    connect(ui->menuRecent, &QMenu::aboutToShow, this, &MainWindow::updateRecentFileActions);
    connect(ui->menuWindow, &QMenu::aboutToShow, this, &MainWindow::updateWindowMenu);
    connect(ui->menuTools, &QMenu::aboutToShow, this, &MainWindow::updateToolsMenu);
    connect(ui->actionClose, &QAction::triggered, ui->mdiArea, &QMdiArea::closeActiveSubWindow);
    connect(ui->actionCloseAll, &QAction::triggered, ui->mdiArea, &QMdiArea::closeAllSubWindows);
    connect(ui->actionTile, &QAction::triggered, ui->mdiArea, &DocumentArea::tileSubWindows);
    connect(ui->actionCascade, &QAction::triggered, ui->mdiArea, &DocumentArea::cascadeSubWindows);
    connect(ui->actionTabbed, &QAction::triggered, ui->mdiArea, &DocumentArea::tabifySubWindows);
    connect(ui->actionNext, &QAction::triggered, ui->mdiArea, &QMdiArea::activateNextSubWindow);
    connect(ui->actionPrevious, &QAction::triggered, ui->mdiArea, &QMdiArea::activatePreviousSubWindow);

    // Tools connections
    connect(ui->actionConversionTool, &QAction::triggered, this, &MainWindow::showConversionDialog);
    connect(ui->actionCodeGenerationTool, &QAction::triggered, this, &MainWindow::showCodeGenerationDialog);
}

void MainWindow::initialise()
{
    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActs[i] = ui->menuRecent->addAction(QString(), this, &MainWindow::openRecentFile);
        recentFileActs[i]->setVisible(false);
    }
}

void MainWindow::aboutActionTriggered()
{
    AboutDialog dlg(this);
    dlg.exec();
}

void MainWindow::openActionTriggered()
{
    const QString settingsPath = "MainWindow/open";
    QString fileName =  QFileDialog::getOpenFileName(this, "Open Document", previousLocation(settingsPath),
                                                     "CellML files (*.cellml *.xml) ;; All files (*.*)");
    if( !fileName.isNull() ) {
        openFile(fileName);
        setPreviousLocation(settingsPath, fileDirectory(fileName));
    }
}

bool MainWindow::openFile(const QString &fileName)
{
    if (QMdiSubWindow *existing = findMdiChild(fileName)) {
        ui->mdiArea->setActiveSubWindow(existing);
        return true;
    }
    const bool succeeded = loadFile(fileName);
    if (succeeded) {
        statusBar()->showMessage(tr("File loaded"), 2000);
    }
    return succeeded;
}

bool MainWindow::loadFile(const QString &fileName)
{
    DocumentWindow *child = createMdiChild();
    const bool succeeded = child->loadFile(fileName);
    if (succeeded) {
        child->show();
    } else {
        child->close();
    }
    MainWindow::prependToRecentFiles(fileName);
    return succeeded;
}

DocumentWindow *MainWindow::createMdiChild()
{
    DocumentWindow *child = new DocumentWindow;
    connect(child, &DocumentWindow::convertFileRequested, this, &MainWindow::convertFileRequestTriggered);
    connect(child, &DocumentWindow::codeGenerationRequested, this, &MainWindow::codeGenerationFileRequestTriggered);
    ui->mdiArea->addSubWindow(child);

    return child;
}

QMdiSubWindow *MainWindow::findMdiChild(const QString &fileName) const
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QMdiSubWindow *window, ui->mdiArea->subWindowList()) {
        DocumentWindow *mdiChild = qobject_cast<DocumentWindow *>(window->widget());
        if (mdiChild->currentFile() == canonicalFilePath) {
            return window;
        }
    }
    return nullptr;
}

void MainWindow::loadNewWindow(const QString& text)
{
    DocumentWindow *child = createMdiChild();
    if (child->loadText(text)) {
        child->show();
    } else {
        child->close();
    }
}

void MainWindow::writeSettings()
{
    QSettings settings;

    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
}

void MainWindow::readSettings()
{
    QSettings settings;

    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(400, 400)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    settings.endGroup();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}

void MainWindow::updateToolsMenu()
{
    ui->menuTools->clear();
    ui->menuTools->addAction(ui->actionConversionTool);
    ui->menuTools->addAction(ui->actionCodeGenerationTool);

}

void MainWindow::updateWindowMenu()
{
    ui->menuWindow->clear();
    ui->menuWindow->addAction(ui->actionClose);
    ui->menuWindow->addAction(ui->actionCloseAll);
    ui->menuWindow->addSeparator();
    ui->menuWindow->addAction(ui->actionTile);
    ui->menuWindow->addAction(ui->actionCascade);
    ui->menuWindow->addAction(ui->actionTabbed);
    ui->menuWindow->addSeparator();
    ui->menuWindow->addAction(ui->actionNext);
    ui->menuWindow->addAction(ui->actionPrevious);

    QList<QMdiSubWindow *> windows = ui->mdiArea->subWindowList();
    if (windows.isEmpty()) {
        ui->menuWindow->setEnabled(false);
    } else {
        ui->menuWindow->setEnabled(true);
        ui->menuWindow->addSeparator();
    }

    for (int i = 0; i < windows.size(); ++i) {
        QMdiSubWindow *mdiSubWindow = windows.at(i);
        DocumentWindow *child = qobject_cast<DocumentWindow *>(mdiSubWindow->widget());

        QString text;
        if (i < 9) {
            text = tr("&%1 %2").arg(i + 1)
                               .arg(child->userFriendlyCurrentFile());
        } else {
            text = tr("%1 %2").arg(i + 1)
                              .arg(child->userFriendlyCurrentFile());
        }
        QAction *action = ui->menuWindow->addAction(text, mdiSubWindow, [this, mdiSubWindow]() {
            ui->mdiArea->setActiveSubWindow(mdiSubWindow);
        });
        action->setCheckable(true);
        action ->setChecked(child == activeMdiChild());
    }
}

void MainWindow::convertFileRequestTriggered(const QString& fileName)
{
    ConversionDialog dlg(fileName, this);
    dlg.setModal(false);
    dlg.exec();
}

void MainWindow::codeGenerationFileRequestTriggered(libcellml::GeneratorProfile::Profile profile, const QString& fileName)
{
    CodeGenerationDialog dlg(profile, fileName, this);
    connect(&dlg, &CodeGenerationDialog::loadText, this, &MainWindow::loadNewWindow);
    dlg.exec();
}

DocumentWindow *MainWindow::activeMdiChild() const
{
    if (QMdiSubWindow *activeSubWindow = ui->mdiArea->activeSubWindow()) {
        return qobject_cast<DocumentWindow *>(activeSubWindow);
    }
    return nullptr;
}


static inline QString recentFilesKey() { return QStringLiteral("recentFileList"); }
static inline QString fileKey() { return QStringLiteral("file"); }

static QStringList readRecentFiles(QSettings &settings)
{
    QStringList result;
    const int count = settings.beginReadArray(recentFilesKey());
    for (int i = 0; i < count; ++i) {
        settings.setArrayIndex(i);
        result.append(settings.value(fileKey()).toString());
    }
    settings.endArray();
    return result;
}

static void writeRecentFiles(const QStringList &files, QSettings &settings)
{
    const int count = files.size();
    settings.beginWriteArray(recentFilesKey());
    for (int i = 0; i < count; ++i) {
        settings.setArrayIndex(i);
        settings.setValue(fileKey(), files.at(i));
    }
    settings.endArray();
}

bool MainWindow::hasRecentFiles()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const int count = settings.beginReadArray(recentFilesKey());
    settings.endArray();
    return count > 0;
}

void MainWindow::prependToRecentFiles(const QString &fileName)
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());

    const QStringList oldRecentFiles = readRecentFiles(settings);
    QStringList recentFiles = oldRecentFiles;
    recentFiles.removeAll(fileName);
    recentFiles.prepend(fileName);
    if (oldRecentFiles != recentFiles) {
        writeRecentFiles(recentFiles, settings);
    }

    setRecentFilesVisible(!recentFiles.isEmpty());
}

void MainWindow::setRecentFilesVisible(bool visible)
{
    ui->menuRecent->setEnabled(visible);
}

void MainWindow::updateRecentFileActions()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());

    const QStringList recentFiles = readRecentFiles(settings);
    const int count = qMin(int(MaxRecentFiles), recentFiles.size());
    int i = 0;
    for ( ; i < count; ++i) {
        const QString fileName = QFileInfo(recentFiles.at(i)).fileName();
        recentFileActs[i]->setText(tr("&%1 %2").arg(i + 1).arg(fileName));
        recentFileActs[i]->setData(recentFiles.at(i));
        recentFileActs[i]->setVisible(true);
    }
    for ( ; i < MaxRecentFiles; ++i)
        recentFileActs[i]->setVisible(false);
}

void MainWindow::openRecentFile()
{
    if (const QAction *action = qobject_cast<const QAction *>(sender())) {
        openFile(action->data().toString());
    }
}

void MainWindow::showConversionDialog()
{
    ConversionDialog dlg(this);
    dlg.setModal(false);
    dlg.exec();
}

void MainWindow::showCodeGenerationDialog()
{
    CodeGenerationDialog dlg(this);
    dlg.exec();
}
