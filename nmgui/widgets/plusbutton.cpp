#include "plusbutton.h"

namespace Nutmeg
{

PlusButton::PlusButton(QWidget *parent)
    : PushButton(parent)
{
    QPalette pal;
    setText("+");
    setFont(QFont("Arial", 12, QFont::Bold));
    setMaximumWidth(22);
    pal.setColor(QPalette::Window, 0xA9A9A9);
    pal.setColor(QPalette::Text, Qt::white);
    setPalette(pal);
}

} // namespace Nutmeg
