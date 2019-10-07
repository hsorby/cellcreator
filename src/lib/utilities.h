#pragma once

#include <QString>
#include <QMimeData>

void writeToFile(const QString &fileName, const QString &content);

QMimeData *setMimeDataText(const QString& text, bool isExistingFileName = false);

QString strippedName(const QString &fullFileName);

QString fileContents(const QString &fullFileName);

QString previousLocation(const QString &settingPath);

void setPreviousLocation(const QString &settingPath, const QString &location);

/**
 * Return the absolute directory path to the given file name.
 *
 * @param fileName The file name to return the path to.
 * @return The absoulte path to the directory part of the given file name.
 */
QString fileDirectory(const QString &fileName);
