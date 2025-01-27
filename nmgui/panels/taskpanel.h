#ifndef NUTMEG_TASKPANEL_H
#define NUTMEG_TASKPANEL_H

#include <QComboBox>
#include <QPalette>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

#include "objects/entity.h"
#include "objects/matter.h"
#include "objects/person.h"
#include "objects/response.h"

#include "frame.h"

#include "widgets/docketnumberbutton.h"
#include "widgets/pushbutton.h"
#include "widgets/tasktypecombo.h"
#include "widgets/titleedit.h"
#include "widgets/informationbox.h"
#include "widgets/arrowbutton.h"

namespace Nutmeg
{

class TaskPanel : public Frame
{
    Q_OBJECT

  public:
    TaskPanel(std::shared_ptr<Task> tsk, QWidget *parent = nullptr);
    TaskPanel(PushButton *xtra, std::shared_ptr<Task>tsk, QWidget *parent = nullptr);

  protected slots:
    void slotUpdateTitle(void);
    void slotUpdateTaskType(Key newval);
    void slotOpenTaskWindow(void);
    void slotOpenMatterWindow(void);
    virtual void slotExtraButtonAction(void) = 0;

  protected:
    // Response *response;
    std::shared_ptr<Matter> matter;
    std::shared_ptr<Task> task;

    std::shared_ptr<Entity> client;
    std::shared_ptr<Person> paralegal;
    std::shared_ptr<Person> workAttorney;
    DocketNumberButton *docketNumberButton;
    ArrowButton *openTaskButton;
    TaskTypeCombo *taskTypeCombo;
    TitleEdit *titleEdit;
    PushButton *extraButton = nullptr;

  private:
    void SetupDisplay(void);
    void ConnectSigalsAndSlots(void);
};

} // namespace Nutmeg
#endif // NUTMEG_RESPONSETASKPANEL_H
