#ifndef DOCUMENTAREA_H
#define DOCUMENTAREA_H

#include <QMdiArea>

class DocumentArea : public QMdiArea
{
    Q_OBJECT

public:
    explicit DocumentArea(QWidget *parent=nullptr);

public slots:
    void tabifySubWindows();
    void cascadeSubWindows();
    void tileSubWindows();
};

#endif // DOCUMENTAREA_H
