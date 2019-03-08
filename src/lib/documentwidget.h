#ifndef DOCUMENTWIDGET_H
#define DOCUMENTWIDGET_H

#include <QWidget>

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

private:
    Ui::DocumentWidget *ui;
};

#endif // DOCUMENTWIDGET_H
