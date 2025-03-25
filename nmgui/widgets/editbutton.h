#ifndef NUTMEG_EDITBUTTON_H
#define NUTMEG_EDITBUTTON_H

#include "pushbutton.h"
#include <QObject>
#include <QWidget>

namespace Nutmeg
{

class EditButton : public Nutmeg::PushButton
{
    Q_OBJECT
  public:
    EditButton(QWidget *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_EDITBUTTON_H
