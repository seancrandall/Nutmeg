#ifndef NUTMEG_PLUSBUTTON_H
#define NUTMEG_PLUSBUTTON_H

#include "pushbutton.h"
#include <QObject>
#include <QWidget>

namespace Nutmeg
{

class PlusButton : public Nutmeg::PushButton
{
    Q_OBJECT
  public:
    PlusButton(QWidget *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_PLUSBUTTON_H
