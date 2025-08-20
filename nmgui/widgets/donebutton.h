#ifndef NUTMEG_DONEBUTTON_H
#define NUTMEG_DONEBUTTON_H

#include "objects/task.h"
#include "objects/responsesdashboardentry.h"
#include "pushbutton.h"
#include <QObject>
#include <QWidget>

namespace Nutmeg
{

class DoneButton : public Nutmeg::PushButton
{
    Q_OBJECT
  public:
    DoneButton(std::shared_ptr<Task> task, QWidget *parent = nullptr);
    DoneButton(const responsesDashboardEntry &entry, QWidget *parent = nullptr);

  public slots:
    void slotHandleClicked(void);

  protected:
    std::shared_ptr<Task> mTask;
};

} // namespace Nutmeg

#endif // NUTMEG_DONEBUTTON_H
