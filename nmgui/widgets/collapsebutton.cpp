#include "collapsebutton.h"

namespace Nutmeg {

CollapseButton::CollapseButton(QWidget *parent)
    : PushButton(parent), mState(CollapseButtonState::Collapsed)
{
    QFont font;
    font.setPointSize(16);
    setFont(font);
    updateGraphic();
    QObject::connect(this, &QPushButton::clicked, this, &CollapseButton::toggle);
}

CollapseButton::CollapseButton(CollapseButtonState initialState, QWidget *parent)
    : PushButton(parent), mState(initialState)
{
    QFont font;
    font.setPointSize(16);
    setFont(font);
    updateGraphic();
    QObject::connect(this, &QPushButton::clicked, this, &CollapseButton::toggle);
}

void CollapseButton::expand()
{
    setState(CollapseButtonState::Expanded);
}

void CollapseButton::collapse()
{
    setState(CollapseButtonState::Collapsed);
}

void CollapseButton::setState(CollapseButtonState newState)
{
    if (mState != newState) {  // Avoid redundant updates
        mState = newState;
        updateGraphic();
        if (mState == CollapseButtonState::Expanded) {
            emit expanded();
        } else {
            emit collapsed();
        }
    }
}

void CollapseButton::toggle()
{
    setState(mState == CollapseButtonState::Collapsed ?
             CollapseButtonState::Expanded :
             CollapseButtonState::Collapsed);
}

void CollapseButton::updateGraphic()
{
    setText(mState == CollapseButtonState::Expanded ? "⮟" : "⮞");
}

} // namespace Nutmeg
