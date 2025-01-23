#include "arrowbutton.h"

namespace Nutmeg
{

ArrowButton::ArrowButton(QWidget *parent) : Nutmeg::PushButton(parent)
{
    setText("‣");
    setFont(QFont("Arial", 10, QFont::Bold));
    setMaximumWidth(25);
}

} // namespace Nutmeg
