#include "droparea.h"

#include <QDragEnterEvent>
#include <QMimeData>

#include <iostream>

DropArea::DropArea(QWidget *parent)
    : QLabel(parent)
{
    setMinimumSize(200, 200);
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    setAlignment(Qt::AlignCenter);
    setAcceptDrops(true);
    setAutoFillBackground(true);
    clear();
}

void DropArea::dragEnterEvent(QDragEnterEvent *event)
{
    setText(tr("<drop CellML 1.0 of CellML 1.1 model>"));
    setBackgroundRole(QPalette::Highlight);

    event->acceptProposedAction();
}

void DropArea::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void DropArea::dropEvent(QDropEvent *event)
{
    /*
     * I need this to return an xml mime type but it is returning bin??? So not using for now.
     * Simply sending out every drop to be xslted.
    const QMimeData *mimeData = event->mimeData();

    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(mimeData->text(), QMimeDatabase::MatchDefault);
    */

    setBackgroundRole(QPalette::Dark);
    event->acceptProposedAction();

    emit dropped(event->mimeData());
}

void DropArea::dragLeaveEvent(QDragLeaveEvent *event)
{
    clear();
    event->accept();
}

void DropArea::clear()
{
    setText(tr("<drop CellML 1.0 of CellML 1.1 model>"));
    setBackgroundRole(QPalette::Dark);
}
