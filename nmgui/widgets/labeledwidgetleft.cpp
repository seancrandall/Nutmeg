#include "labeledwidgetleft.h"

namespace Nutmeg
{

LabeledWidgetLeft::LabeledWidgetLeft(const QString label, QWidget *widget, QWidget *parent)
    : QHBoxLayout(parent), mLabel(new QLabel(label))
{
    // addStretch();
    addWidget(mLabel);
    addWidget(widget);
}

} // namespace Nutmeg
