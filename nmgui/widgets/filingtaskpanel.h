#ifndef FILINGTASKPANEL_H
#define FILINGTASKPANEL_H

#include <QObject>
#include <QWidget>

#include "objects/filing.h"
#include "taskpanel.h"

namespace Nutmeg
{

class FilingTaskPanel : public Nutmeg::TaskPanel
{
    Q_OBJECT
  public:
    FilingTaskPanel(Filing *fil, QWidget *parent = nullptr);

  signals:
    void signalSomethingChanged(void);

  public slots:
    virtual void slotExtraButtonAction(void) override;
};

} // namespace Nutmeg

#endif // FILINGTASKPANEL_H
