#ifndef COLLAPSEBUTTON_H
#define COLLAPSEBUTTON_H

#include "pushbutton.h"
#include "property.h"

namespace Nutmeg {

enum class CollapseButtonState {
    Expanded,
    Collapsed
};

class CollapseButton : public Nutmeg::PushButton
{
    Q_OBJECT
public:
    CollapseButton(QWidget *parent = nullptr);
    CollapseButton(CollapseButtonState state, QWidget *parent = nullptr);

    Property(getState, setState) CollapseButtonState state;
    CollapseButtonState getState() { return mState; }

signals:
    void expanded();
    void collapsed();
    void toggled();

public slots:
    void expand();
    void collapse();
    void setState(CollapseButtonState newState);
    void toggle();

private:
    CollapseButtonState mState;
    void updateGraphic();
    void Initialize();
};

} // namespace Nutmeg

#endif // COLLAPSEBUTTON_H
