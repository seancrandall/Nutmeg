#ifndef COLLAPSEBUTTON_H
#define COLLAPSEBUTTON_H

#include "pushbutton.h"
#include "property.h"

namespace Nutmeg {

enum class ExpandButtonState{
    Expanded,
    Collapsed
};

class CollapseButton : public Nutmeg::PushButton
{
    Q_OBJECT
public:
    CollapseButton(QWidget *parent=nullptr);
    CollapseButton(ExpandButtonState state, QWidget *parent=nullptr);

    Property(getState, setState) ExpandButtonState state;
    ExpandButtonState getState(void) {return mState;}

signals:
    void signalExpanded(void);
    void signalCollapsed(void);

public slots:
    void Expand(void);
    void Collapse(void);
    void setState(ExpandButtonState newstate);

private:
    ExpandButtonState mState;
    void UpdateGraphic(void);
    void Initialize(void);
};

} // namespace Nutmeg

#endif // COLLAPSEBUTTON_H
