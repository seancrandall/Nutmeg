#ifndef NUTMEG_ABSTRACTTASKPANEL_H
#define NUTMEG_ABSTRACTTASKPANEL_H

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
#include "widgets/disabledbutton.h"

#include "windows/taskdialog.h"

namespace Nutmeg
{

class AbstractTaskPanel : public Frame
{
    Q_OBJECT

  public:
    AbstractTaskPanel(std::shared_ptr<Task> task, QWidget *parent = nullptr);
    AbstractTaskPanel(PushButton *xtra, std::shared_ptr<Task> task, QWidget *parent = nullptr);
    AbstractTaskPanel(Key taskId, PushButton *xtra, QWidget *parent = nullptr);
    AbstractTaskPanel(Key appointmentId, QWidget *parent = nullptr);

    Property(getExtraButton, setExtraButton) PushButton *extraButton;

    PushButton *getExtraButton(void) const {return mExtraButton;}
    void setExtraButton(PushButton* newButton);

  protected slots:
    void slotUpdateTitle(void);
    void slotUpdateTaskType(Key newval);
    void slotOpenTaskWindow(void);
    void slotOpenMatterWindow(void);
    virtual void slotExtraButtonAction(void) = 0;

  protected:
    std::shared_ptr<Matter> mMatter;
    std::shared_ptr<Task> mTask;

    DocketNumberButton *docketNumberButton;
    ArrowButton *openTaskButton;
    TaskTypeCombo *taskTypeCombo;
    TitleEdit *titleEdit;
    PushButton *mExtraButton = nullptr;

  private:
    void ConnectSigalsAndSlots(void);
    void SetupDisplay(void);
};

} // namespace Nutmeg
#endif // NUTMEG_ABSTRACTRESPONSETASKPANEL_H
