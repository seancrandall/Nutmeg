#ifndef NUTMEG_DEADLINESPANEL_H
#define NUTMEG_DEADLINESPANEL_H

#include <QDateEdit>
#include <QLabel>
#include <QObject>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFrame>

#include "objects/deadline.h"
#include "widgets/dateedit.h"
#include "frame.h"

namespace Nutmeg
{

/*******************************************************************
 *  TODO : Do Scatter and Gather so we don't auto-update everything
 * *****************************************************************/
class DeadlinesPanel : public Frame
{
    Q_OBJECT
  public:
    explicit DeadlinesPanel(std::shared_ptr<Deadline> deadline, QWidget *parent = nullptr);

  signals:

  protected:
    std::shared_ptr<Deadline> mDeadline;

    Nutmeg::DateEdit *triggerEdit, *nextEdit, *softEdit, *hardEdit;

  protected slots:
    void slotUpdateTriggerDate(void);
    void slotUpdateNextDeadline(void);
    void slotUpdateSoftDeadline(void);
    void slotUpdateHardDeadline(void);

  private:
    void ConnectSignalsAndSlots(void);
};

} // namespace Nutmeg

#endif // NUTMEG_DEADLINESPANEL_H
