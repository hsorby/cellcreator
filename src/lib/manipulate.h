
#pragma once

#include <sstream>

#include <QString>
#include <QMimeData>

bool xsltTransfrom(const QString &url, QString *out, QString *msg);

bool isCellML1(const QString &url, QString *out, QString *msg);

bool isCellML2(const QString &url, QString *out, QString *msg);

std::string generateCode(const std::string &text);

std::string libCellMLPrintModel(const std::string &text);

std::string parseText(const std::string &text);

void initialiseResources();
