#include "timeedit.h"

namespace Nutmeg {

TimeEdit::TimeEdit(QWidget *parent)
    : QTimeEdit(parent)
{
    setDisplayFormat("hh:mm ap");
}

TimeEdit::TimeEdit(const QTime &newtime, QWidget *parent)
    : QTimeEdit(newtime, parent)
{
    setDisplayFormat("hh:mm ap");
}

} // namespace Nutmeg
