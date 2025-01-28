#ifndef NUTMEG_DOCKETNUMBERBUTTON_H
#define NUTMEG_DOCKETNUMBERBUTTON_H

#include <QObject>
#include <QWidget>

#include "objects/matter.h"
#include "pushbutton.h"

namespace Nutmeg
{

class DocketNumberButton : public Nutmeg::PushButton
{
    Q_OBJECT
  public:
    DocketNumberButton(Matter* matter, QWidget *parent);

  protected:
    Matter* mMatter;
};

} // namespace Nutmeg

#endif // NUTMEG_DOCKETNUMBERBUTTON_H
