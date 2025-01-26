#ifndef NUTMEG_RESPONSETASKPANEL_H
#define NUTMEG_RESPONSETASKPANEL_H

#include "objects/response.h"
#include "taskpanel.h"
#include <QObject>
#include <QWidget>

#include "windows/addexaminerinterviewdialog.h"

namespace Nutmeg
{

class ResponseTaskPanel : public Nutmeg::TaskPanel
{
    Q_OBJECT
  public:
    ResponseTaskPanel(std::shared_ptr<Response> rsp, QWidget *parent = nullptr);

  signals:
    void signalSomethingChanged(void);

  public slots:
    virtual void slotExtraButtonAction(void) override;

  protected:
    std::shared_ptr<Response> response;
};

} // namespace Nutmeg

#endif // NUTMEG_RESPONSETASKPANEL_H
