#include "documentwindow.h"

#include "documentwidget.h"

#include <QtWidgets>

DocumentWindow::DocumentWindow(QWidget *parent)
 : QWidget(parent)
{
    widget = new DocumentWidget();
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(widget);
    layout->setMargin(0);
    setLayout(layout);

    makeConnections();
}

DocumentWindow::~DocumentWindow()
{
}

void  DocumentWindow::makeConnections()
{
    connect(widget, &DocumentWidget::conversionRequested, this, &DocumentWindow::conversionRequestTriggered);
}

bool DocumentWindow::loadFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Document Window"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    widget->setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);

    return true;
}

void DocumentWindow::conversionRequestTriggered()
{
    emit convertFileRequested(curFile);
}

void DocumentWindow::setCurrentFile(const QString &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    isUntitled = false;
    //widget->document()->setModified(false);
    setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
}

QString DocumentWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

QString DocumentWindow::userFriendlyCurrentFile()
{
    return strippedName(curFile);
}
