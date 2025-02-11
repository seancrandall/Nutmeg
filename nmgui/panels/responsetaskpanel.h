#ifndef NUTMEG_RESPONSETASKPANEL_H
#define NUTMEG_RESPONSETASKPANEL_H

#include "objects/response.h"
#include "abstracttaskpanel.h"
#include <QObject>
#include <QWidget>

#include "windows/addexaminerinterviewdialog.h"

namespace Nutmeg
{

class ResponseTaskPanel : public Nutmeg::AbstractTaskPanel
{
    Q_OBJECT
  public:
    ResponseTaskPanel(std::shared_ptr<Response> response, QWidget *parent = nullptr);

  signals:
    void signalSomethingChanged(void);

  public slots:
    virtual void slotExtraButtonAction(void);

  protected:
    std::shared_ptr<Response> mResponse;
};

} // namespace Nutmeg

#endif // NUTMEG_RESPONSETASKPANEL_H
