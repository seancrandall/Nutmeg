#ifndef FLAGSPANEL_H
#define FLAGSPANEL_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>

#include "frame.h"
#include "objects/object.h"
#include "nutmeg.h"
#include "widgets/flagcheckbox.h"

namespace Nutmeg
{

class FlagsPanel : public Nutmeg::Frame
{
    Q_OBJECT
  public:
    FlagsPanel(Key objectid, QWidget *parent=nullptr);

  protected:
    Nutmeg::Object object;
};

} // namespace Nutmeg

#endif // FLAGSPANEL_H
