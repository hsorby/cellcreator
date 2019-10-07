#include "utilities.h"

#include <QtCore>

void writeToFile(const QString &fileName, const QString &content)
{
    if (fileName.length() > 0) {
        QFile outFile(fileName);
        if (outFile.open(QIODevice::WriteOnly)) {
            outFile.write(content.toUtf8().constData());
        }
        outFile.close();
    }
}

QMimeData *setMimeDataText(const QString& text, bool isExistingFileName)
{
    QMimeData *mimeData = nullptr;
    if (isExistingFileName) {
        QFile file(text);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            return mimeData;
        }
        file.close();
    }

    mimeData = new QMimeData;
    mimeData->setText(text);

    return mimeData;
}

QString strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

QString fileContents(const QString &fullFileName)
{
    QString contents;
    QFile file(fullFileName);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&file);
        contents = in.readAll();
    }
    file.close();

    return contents;
}

QString previousLocation(const QString &settingPath)
{
    QSettings settings;

    settings.beginGroup(settingPath);
    QString location = settings.value("location", QDir::homePath()).toString();
    settings.endGroup();

    return location;
}

void setPreviousLocation(const QString &settingPath, const QString &location)
{
    QSettings settings;

    settings.beginGroup(settingPath);
    settings.setValue("location", location);
    settings.endGroup();
}

QString fileDirectory(const QString &fileName)
{
    QDir d = QFileInfo(fileName).absoluteDir();
    QString absolutePath = d.absolutePath();

    return absolutePath;
}
