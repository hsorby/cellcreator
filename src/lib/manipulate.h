
#pragma once

#include <sstream>

#include <QString>

bool xsltTransfrom(const QString &url, QString *out, QString *msg);

std::string libCellMLPrintModel(const std::string &text);

std::string parseText(const std::string &text);

void writeToFile(const QString &fileName, const QString &content);

void initialiseResources();
