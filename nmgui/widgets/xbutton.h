#ifndef NUTMEG_XBUTTON_H
#define NUTMEG_XBUTTON_H

#include <QObject>
#include <QWidget>

#include "widgets/pushbutton.h"

namespace Nutmeg
{

class XButton : public Nutmeg::PushButton
{
    Q_OBJECT
  public:
    XButton(QWidget *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_XBUTTON_H
