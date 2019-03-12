#pragma once

#include <QString>
#include <QMimeData>

void writeToFile(const QString &fileName, const QString &content);

QMimeData *setMimeDataText(const QString& text, bool isExistingFileName = false);
