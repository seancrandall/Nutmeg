#ifndef FILINGTASKPANEL_H
#define FILINGTASKPANEL_H

#include <QObject>
#include <QWidget>

#include "objects/filing.h"
#include "abstracttaskpanel.h"

namespace Nutmeg
{

class FilingTaskPanel : public Nutmeg::AbstractTaskPanel
{
    Q_OBJECT
  public:
    FilingTaskPanel(std::shared_ptr<Filing> fil, QWidget *parent = nullptr);

  signals:
    void signalSomethingChanged(void);

  public slots:
    virtual void slotExtraButtonAction(void) override;
};

} // namespace Nutmeg

#endif // FILINGTASKPANEL_H
