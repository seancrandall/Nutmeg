#include "pushbutton.h"

namespace Nutmeg
{

PushButton::PushButton(QWidget *parent) : QPushButton(parent)
{
    NUTMEGFONT;

    QPalette pal;
    pal.setColor(QPalette::Window, Qt::gray);
    setPalette(pal);
    setAutoFillBackground(true);

    setFlat(true);

    setMaximumWidth(300);
}

} // namespace Nutmeg
