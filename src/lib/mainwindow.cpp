
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <sstream>

#include <QXmlQuery>

#include <libcellml>

#include "aboutdialog.h"
#include "manipulate.h"
#include "documentwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    readSettings();
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
}

void MainWindow::aboutActionTriggered()
{
    AboutDialog dlg(this);
    dlg.exec();
}

void MainWindow::openActionTriggered()
{
    QString fileName =  QFileDialog::getOpenFileName(this, "Open Document", QDir::currentPath(),
                                                     "CellML files (*.cellml *.xml) ;; All files (*.*)");
    if( !fileName.isNull() )
    {
        openFile(fileName);
    }
}

bool MainWindow::openFile(const QString &fileName)
{
    if (QMdiSubWindow *existing = findMdiChild(fileName)) {
        ui->mdiArea->setActiveSubWindow(existing);
        return true;
    }
    const bool succeeded = loadFile(fileName);
    if (succeeded)
        statusBar()->showMessage(tr("File loaded"), 2000);
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
    ui->mdiArea->addSubWindow(child);

    return child;
}


QMdiSubWindow *MainWindow::findMdiChild(const QString &fileName) const
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QMdiSubWindow *window, ui->mdiArea->subWindowList()) {
        DocumentWindow *mdiChild = qobject_cast<DocumentWindow *>(window->widget());
        if (mdiChild->currentFile() == canonicalFilePath)
            return window;
    }
    return nullptr;
}

void MainWindow::writeSettings()
{
    QSettings settings;

    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
    settings.beginGroup("UserInterface");
    settings.beginGroup("Xslt");
//    settings.setValue("fileName", ui->lineEditOutputFileXsltTransformed->text());
    settings.endGroup();
    settings.beginGroup("Printed");
//    settings.setValue("fileName", ui->lineEditOutputFileLibCellMLPrinted->text());
    settings.endGroup();
    settings.endGroup();
}

void MainWindow::readSettings()
{
    QSettings settings;

    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(400, 400)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    settings.endGroup();
    settings.beginGroup("UserInterface");
    settings.beginGroup("Xslt");
//    ui->lineEditOutputFileXsltTransformed->setText(settings.value("fileName", QString("")).toString());
    settings.endGroup();
    settings.beginGroup("Printed");
//    ui->lineEditOutputFileLibCellMLPrinted->setText(settings.value("fileName", QString("")).toString());
    settings.endGroup();
    settings.endGroup();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
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
    if (oldRecentFiles != recentFiles)
        writeRecentFiles(recentFiles, settings);

    setRecentFilesVisible(!recentFiles.isEmpty());
}

void MainWindow::setRecentFilesVisible(bool visible)
{
    recentFileSubMenuAct->setVisible(visible);
    recentFileSeparator->setVisible(visible);
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
    if (const QAction *action = qobject_cast<const QAction *>(sender()))
        openFile(action->data().toString());
}
