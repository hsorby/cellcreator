
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
        }
        xslt.close();
    }

    *msg = messageHandler.statusMessage();

    return success;
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
            os << "<li>" << parser.getError(i)->getDescription() << "</li>" << std::endl;
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
            os << "<li>" << validator.getError(i)->getDescription() << "</li>" << std::endl;
        }
        os << "</ol>" << std::endl;
    }
    os << std::endl << std::endl;
    os << "</body></html>" << std::endl;

    return os.str();
}

std::string libCellMLPrintModel(const std::string &text)
{
    libcellml::Printer printer;
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(text);

    return printer.printModel(model);
}

void writeToFile(const QString &fileName, const QString &content)
{
    if (fileName.length() > 0) {
        QFile outFile(fileName);
        if (outFile.open(QIODevice::WriteOnly)) {
            outFile.write(content.toUtf8().constData());
            outFile.close();
        }
    }
}

void initialiseResources()
{
    Q_INIT_RESOURCE(resources);
}
