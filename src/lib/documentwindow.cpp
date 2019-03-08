#include "documentwindow.h"

#include "documentwidget.h"

#include <QtWidgets>

DocumentWindow::DocumentWindow(QWidget *parent)
 : QMdiSubWindow(parent)
{
    widget = new DocumentWidget(this);
    this->setWidget(widget);
}

DocumentWindow::~DocumentWindow()
{
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

//    connect(document(), &QTextDocument::contentsChanged,
//            this, &MdiChild::documentWasModified);

    return true;

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
