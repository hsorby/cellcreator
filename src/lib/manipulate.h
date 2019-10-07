
#pragma once

#include <sstream>

#include <QString>
#include <QMimeData>

#include <libcellml>

struct GeneratedCode
{
    libcellml::GeneratorProfile::Profile profile;
    std::string implementationCode;
    std::string interfaceCode;
};

bool xsltTransfrom(const QString &url, QString *out, QString *msg);

bool isCellML1(const QString &url, QString *out, QString *msg);

bool isCellML2(const QString &url, QString *out, QString *msg);

GeneratedCode generateCode(libcellml::GeneratorProfile::Profile profile, const std::string &text);

std::string libCellMLPrintModel(const std::string &text);

std::string parseText(const std::string &text);

void initialiseResources();

void registerMetatypes();
