#include "docketnumberbutton.h"

namespace Nutmeg
{

DocketNumberButton::DocketNumberButton(Matter *matter, QWidget *parent)
    : Nutmeg::PushButton(parent)
    , mMatter(matter)
{
    setText(matter->AttorneyDocketNumber);
    // setFixedWidth(200);
}

} // namespace Nutmeg
