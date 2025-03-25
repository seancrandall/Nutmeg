#ifndef DISABLEDBUTTON_H
#define DISABLEDBUTTON_H

#include "pushbutton.h"

namespace Nutmeg {

class DisabledButton : public Nutmeg::PushButton
{
    Q_OBJECT
public:
    DisabledButton(QWidget *parent=nullptr);
    DisabledButton(const QString& buttonText, QWidget *parent=nullptr);

    void Initialize();
};

} // namespace Nutmeg

#endif // DISABLEDBUTTON_H
