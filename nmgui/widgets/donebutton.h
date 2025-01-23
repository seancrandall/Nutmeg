#ifndef NUTMEG_DONEBUTTON_H
#define NUTMEG_DONEBUTTON_H

#include "objects/task.h"
#include "pushbutton.h"
#include <QObject>
#include <QWidget>

namespace Nutmeg
{

class DoneButton : public Nutmeg::PushButton
{
    Q_OBJECT
  public:
    DoneButton(Task *task, QWidget *parent = nullptr);

  public slots:
    void slotHandleClicked(void);

  protected:
    Task *mTask;
};

} // namespace Nutmeg

#endif // NUTMEG_DONEBUTTON_H
