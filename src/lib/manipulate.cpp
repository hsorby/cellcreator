
#include "manipulate.h"

#include <QtCore>
#include <QXmlQuery>

#include <libcellml>

#include "messagehandler.h"

bool xsltTransfrom(const QString &url, QString *out, QString *msg)
{
    MessageHandler messageHandler;

    bool success = false;
    QFile xslt(":/xslt/cellml1to2.xsl");

    if (xslt.open(QIODevice::ReadOnly)) {
        QXmlQuery query(QXmlQuery::XSLT20);
        query.setMessageHandler(&messageHandler);
        success = query.setFocus(QUrl(url));
        if (success) {
            query.setQuery(xslt.readAll());
            success = query.evaluateTo(out);
            // Having an issue with empty namespaces when using QXmlQuery.
            // Simply removing the offending namespace for the time being.
            out->replace(" xmlns=\"\"", "");
            out->replace("cn units", "cn cellml:units");
            if (out->contains("cellml:units") && !out->contains(" xmlns:cellml=\"http://www.cellml.org/cellml/2.0#\"")) {
                out->replace("xmlns=\"http://www.cellml.org/cellml/2.0#\"", "xmlns=\"http://www.cellml.org/cellml/2.0#\" xmlns:cellml=\"http://www.cellml.org/cellml/2.0#\"");
            }
        }
        xslt.close();
    }

    *msg = messageHandler.statusMessage();

    return success;
}

bool isCellML(const QString &url, QString *out, QString *msg, QFile &xslt)
{
    MessageHandler messageHandler;
    bool success = false;
    bool isCellmlFile = false;

    if (xslt.open(QIODevice::ReadOnly)) {
        QXmlQuery query(QXmlQuery::XSLT20);
        query.setMessageHandler(&messageHandler);
        success = query.setFocus(QUrl(url));
        if (success) {
            query.setQuery(xslt.readAll());
            success = query.evaluateTo(out);
        }
        xslt.close();
    }

    if (success && out->contains("model")) {
        isCellmlFile = true;
    }
    *msg = messageHandler.statusMessage();

    return isCellmlFile;
}

bool isCellML1(const QString &url, QString *out, QString *msg)
{
    QFile xslt(":/xslt/iscellml1.xsl");
    return isCellML(url, out, msg, xslt);
}

bool isCellML2(const QString &url, QString *out, QString *msg)
{
    QFile xslt(":/xslt/iscellml2.xsl");
    return isCellML(url, out, msg, xslt);
}

std::string parseText(const std::string &text)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(text);

    std::ostringstream os;
    os << "<html xmlns='http://www.w3.org/1999/xhtml/'><body>" << std::endl;
    os << "<h1><bold>Parsing errors</bold></h1>" << std::endl;
    if (parser.errorCount() == 0) {
        os << "<ul><li>No parsing errors.</li></ul>" << std::endl;
    } else {
        os << "<ol>" << std::endl;
        for (size_t i = 0; i < parser.errorCount(); ++i) {
            os << "<li>" << parser.error(i)->description() << "</li>" << std::endl;
        }
        os << "</ol>" << std::endl;
    }
    libcellml::Validator validator;
    validator.validateModel(model);
    os << std::endl;
    os << "<h1><bold>Validation errors</bold></h1>" << std::endl;
    if (validator.errorCount() == 0) {
        os << "<ul><li>No validation erros.</li></ul>" << std::endl;
    } else {
        os << "<ol>" << std::endl;
        for (size_t i = 0; i < validator.errorCount(); ++i) {
            os << "<li>" << validator.error(i)->description() << "</li>" << std::endl;
        }
        os << "</ol>" << std::endl;
    }
    os << std::endl << std::endl;
    os << "</body></html>" << std::endl;

    return os.str();
}

GeneratedCode generateCode(libcellml::GeneratorProfile::Profile profile, const std::string &text)
{
    libcellml::Parser parser;
    libcellml::Generator generator;
    libcellml::GeneratorProfilePtr generatorProfile = std::make_shared<libcellml::GeneratorProfile>();
    libcellml::ModelPtr model = parser.parseModel(text);

    generatorProfile->setProfile(profile);
    generator.setProfile(generatorProfile);
    generator.processModel(model);

    GeneratedCode code;
    code.profile = profile;
    code.implementationCode = generator.implementationCode();
    code.interfaceCode = generator.interfaceCode();
    return code;
}

std::string libCellMLPrintModel(const std::string &text)
{
    libcellml::Printer printer;
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(text);

    return printer.printModel(model);
}

void initialiseResources()
{
    Q_INIT_RESOURCE(resources);
}

void registerMetatypes()
{
    qRegisterMetaType<libcellml::GeneratorProfile::Profile>("libcellml::GeneratorProfile::Profile");
}
