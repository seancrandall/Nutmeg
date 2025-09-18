#ifndef FLAGSPANEL_H
#define FLAGSPANEL_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>

#include "frame.h"
#include "objects/object.h"
#include "objects/responsesdashboardentry.h"
#include "nutmeg.h"
#include "widgets/flagcheckbox.h"

namespace Nutmeg
{

class FlagsPanel : public Nutmeg::Frame
{
    Q_OBJECT
  public:
    FlagsPanel(Key objectid, QWidget *parent=nullptr);
    FlagsPanel(const responsesDashboardEntry &entry, QWidget *parent=nullptr);

  protected:
    std::shared_ptr<Object> mObject;

  private:
    void SetupPanel(void);
};

} // namespace Nutmeg

#endif // FLAGSPANEL_H
