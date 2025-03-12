#include "collapsebutton.h"

namespace Nutmeg {

CollapseButton::CollapseButton(QWidget *parent)
    : PushButton(parent)
{
    mState = ExpandButtonState::Collapsed;
    Initialize();
}

CollapseButton::CollapseButton(ExpandButtonState state, QWidget *parent)
    : PushButton(parent)
{
    mState = state;
    Initialize();
}

void CollapseButton::Expand()
{
    setState(ExpandButtonState::Expanded);
}

void CollapseButton::Collapse()
{
    setState(ExpandButtonState::Collapsed);
}

void CollapseButton::setState(ExpandButtonState newstate)
{
    mState = newstate;
    Initialize();
}

void CollapseButton::UpdateGraphic()
{
    if(mState == ExpandButtonState::Expanded){
        QPushButton::setText("⮟");
    } else if(mState == ExpandButtonState::Collapsed){
        QPushButton::setText("⮞");
    } else {
        QPushButton::setText("?");
    }
}

void CollapseButton::Initialize()
{
    QFont font;
    font.setPointSize(16);
    QPushButton::setFont(font);
    UpdateGraphic();
}

} // namespace Nutmeg
