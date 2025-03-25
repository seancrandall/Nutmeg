#include "dynamicstackedwidget.h"

namespace Nutmeg {

DynamicStackedWidget::DynamicStackedWidget(QWidget *parent)
    : QStackedWidget(parent)
{
    // Connect the currentChanged signal to updateGeometry to notify the layout
    connect(this, &QStackedWidget::currentChanged, this, &QWidget::updateGeometry);
}

QSize DynamicStackedWidget::sizeHint() const
{
    QWidget *current = currentWidget();
    if (current) {
        return current->sizeHint();
    }
    return QStackedWidget::sizeHint();
}

QSize DynamicStackedWidget::minimumSizeHint() const
{
    QWidget *current = currentWidget();
    if (current) {
        return current->minimumSizeHint();
    }
    return QStackedWidget::minimumSizeHint();
}




} // namespace Nutmeg
