#include "documentarea.h"

#include <QtWidgets>

DocumentArea::DocumentArea(QWidget *parent)
  : QMdiArea(parent)
{
    setViewMode(QMdiArea::TabbedView);
    setTabsClosable(true);
    setTabsMovable(true);
}

void DocumentArea::tabifySubWindows()
{
    setViewMode(QMdiArea::TabbedView);
}

void DocumentArea::cascadeSubWindows()
{
    if (viewMode() == QMdiArea::TabbedView) {
        setViewMode(QMdiArea::SubWindowView);
    }
    QMdiArea::cascadeSubWindows();
}

void DocumentArea::tileSubWindows()
{
    if (viewMode() == QMdiArea::TabbedView) {
        setViewMode(QMdiArea::SubWindowView);
    }
    QMdiArea::tileSubWindows();
}
