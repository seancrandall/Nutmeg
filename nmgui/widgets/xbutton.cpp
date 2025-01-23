#include "xbutton.h"

namespace Nutmeg
{

XButton::XButton(QWidget *parent) : Nutmeg::PushButton(parent)
{
    setFont(QFont("Arial", 12, QFont::Bold));
    setText("⤬");
    setMaximumWidth(22);
}

} // namespace Nutmeg
