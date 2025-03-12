#include "collapsebutton.h"

namespace Nutmeg {

CollapseButton::CollapseButton(QWidget *parent)
    : PushButton(parent), mState(CollapseButtonState::Collapsed)
{
    Initialize();
    QObject::connect(this, &QPushButton::clicked, this, &CollapseButton::toggle);
}

CollapseButton::CollapseButton(CollapseButtonState initialState, QWidget *parent)
    : PushButton(parent), mState(initialState)
{
    Initialize();
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
    emit toggled();
}

void CollapseButton::updateGraphic()
{
    setText(mState == CollapseButtonState::Expanded ? "⮟" : "⮞");
}

void CollapseButton::Initialize()
{
    // Set the font with point size 14
    QFont font;
    font.setPointSize(14);
    setFont(font);

    // Apply stylesheet for transparent background and minimal spacing
    setStyleSheet("background-color: transparent; padding: 0; border: none;");

    // Calculate the width required for the arrow graphics
    QFontMetrics fm(font);
    int widthExpanded = fm.horizontalAdvance("⮟");  // Width of down arrow
    int widthCollapsed = fm.horizontalAdvance("⮞"); // Width of right arrow
    int maxWidth = qMax(widthExpanded, widthCollapsed); // Use the larger width

    // Set the button's fixed width to match the arrow graphic
    setFixedWidth(maxWidth);

    // Set the initial graphic based on the current state
    updateGraphic();
}

} // namespace Nutmeg
