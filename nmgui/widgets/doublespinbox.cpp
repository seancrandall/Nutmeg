#include "doublespinbox.h"

namespace Nutmeg {

DoubleSpinBox::DoubleSpinBox(QWidget *parent)
    : QDoubleSpinBox(parent)
{
    setMaximum(99999.99);
}

} // namespace Nutmeg
