#include "timeedit.h"

namespace Nutmeg {

TimeEdit::TimeEdit(QWidget *parent)
    : QTimeEdit(parent)
{
    setDisplayFormat("hh:mm ap");
}

} // namespace Nutmeg
