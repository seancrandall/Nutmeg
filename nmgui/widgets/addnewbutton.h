#ifndef NUTMEG_ADDNEWBUTTON_H
#define NUTMEG_ADDNEWBUTTON_H

#include "pushbutton.h"
#include <QObject>
#include <QWidget>

namespace Nutmeg
{

class AddNewButton : public Nutmeg::PushButton
{
    Q_OBJECT
  public:
    explicit AddNewButton(QWidget *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_ADDNEWBUTTON_H
