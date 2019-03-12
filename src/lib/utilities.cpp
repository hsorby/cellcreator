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
