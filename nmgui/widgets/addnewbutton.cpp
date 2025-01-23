#include "addnewbutton.h"

namespace Nutmeg
{

AddNewButton::AddNewButton(QWidget *parent) : Nutmeg::PushButton(parent)
{
    QPalette pal;
    setText("Add New");
    pal.setColor(QPalette::Window, "#A9A9A9");
    pal.setColor(QPalette::Text, Qt::white);
    setPalette(pal);
    // setAutoFillBackground(true);

    setFlat(false);
}

} // namespace Nutmeg
