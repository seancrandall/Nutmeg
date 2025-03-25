#ifndef NUTMEG_ARROWBUTTON_H
#define NUTMEG_ARROWBUTTON_H

#include "pushbutton.h"
#include <QObject>
#include <QWidget>

namespace Nutmeg
{

class ArrowButton : public Nutmeg::PushButton
{
    Q_OBJECT
  public:
    ArrowButton(QWidget *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_ARROWBUTTON_H
