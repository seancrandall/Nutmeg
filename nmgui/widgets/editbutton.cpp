#include "editbutton.h"

namespace Nutmeg
{

EditButton::EditButton(QWidget *parent) : Nutmeg::PushButton(parent)
{
    setText("✍");
    setMaximumWidth(22);
}

} // namespace Nutmeg
