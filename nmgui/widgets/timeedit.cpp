#include "timeedit.h"

namespace Nutmeg {

TimeEdit::TimeEdit(QWidget *parent)
    : QTimeEdit(parent)
{
    setDisplayFormat("H:m ap");
}

} // namespace Nutmeg
