#ifndef NUTMEG_PUSHBUTTON_H
#define NUTMEG_PUSHBUTTON_H

#include <QObject>
#include <QPushButton>
#include <QWidget>

#include "nutmeg.h"

namespace Nutmeg
{

class PushButton : public QPushButton
{
    Q_OBJECT
  public:
    PushButton(QWidget *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_PUSHBUTTON_H
