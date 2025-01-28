#include "emailinventorsbutton.h"

namespace Nutmeg
{

EmailInventorsButton::EmailInventorsButton(Filing* fil, QWidget *parent)
    : PushButton(parent)
    , mFiling(fil)
{

    QPushButton::setText("Email Inventors");

    // TODO: Check if examiner interview is complete. If so, hide the button
}

void EmailInventorsButton::slotHandleClick() {}

} // namespace Nutmeg
