#pragma once

#include <QtWidgets>

class DropArea : public QLabel
{
    Q_OBJECT

public:
    explicit DropArea(QWidget *parent = nullptr);

public slots:
    void clear();

signals:
    void dropped(const QMimeData *mimeData = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    QLabel *label;
};
