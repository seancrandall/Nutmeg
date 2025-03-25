#include "disabledbutton.h"

namespace Nutmeg {

DisabledButton::DisabledButton(QWidget *parent)
    : PushButton(parent)
{

}

DisabledButton::DisabledButton(const QString &buttonText, QWidget *parent)
    : PushButton(parent)
{
    setText(buttonText);
    Initialize();
}

void DisabledButton::Initialize()
{
    setDisabled(true);
}

} // namespace Nutmeg
