#ifndef DOCUMENTWIDGET_H
#define DOCUMENTWIDGET_H

#include <QWidget>
#include <QMenu>

namespace Ui {
class DocumentWidget;
}

class DocumentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DocumentWidget(QWidget *parent = nullptr);
    ~DocumentWidget();

    void setPlainText(const QString& text);
    QMenu *createStandardContextMenu();

signals:
    void conversionRequested();
    void codeGenerationRequested();

private slots:
    void customMenuRequested(const QPoint& pos);

private:
    void makeConnections();

private:
    Ui::DocumentWidget *ui;
};

#endif // DOCUMENTWIDGET_H
