#ifndef TASKPANELFULL_H
#define TASKPANELFULL_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>

#include "nutmeg.h"
#include "property.h"

#include "frame.h"
#include "appointmentpanel.h"
#include "deadlinespanel.h"

#include "widgets/dateedit.h"
#include "widgets/checkbox.h"
#include "widgets/labeledwidgetleft.h"
#include "widgets/paralegalsearchbox.h"
#include "widgets/spinbox.h"
#include "widgets/doublespinbox.h"
#include "widgets/workattorneysearchbox.h"
#include "widgets/combobox.h"
#include "widgets/tasktypecombo.h"

#include "objects/task.h"

namespace Nutmeg {

class TaskPanelFull : public Nutmeg::Frame
{
    Q_OBJECT
public:
    TaskPanelFull(Task* task, QWidget *parent = nullptr);
    TaskPanelFull(Key taskid, QWidget *parent = nullptr);

    void Scatter(void);
    void Gather(void);

protected:
    Task* mTask;
    Matter *mMatter;

    //Controls
    QGridLayout *grid;
    DateEdit *cDateAssigned = new DateEdit();
    CheckBox *cComplete = new CheckBox();
    DoubleSpinBox *cExpectedFee = new DoubleSpinBox();
    DoubleSpinBox *cFeePercent = new DoubleSpinBox();
    WorkAttorneySearchBox *cWorkAttorney = new WorkAttorneySearchBox();
    ParalegalSearchBox *cParalegal = new ParalegalSearchBox();
    DeadlinesPanel *cDeadlines;
    TaskTypeCombo *cTaskType;

    //Layouts
    LabeledWidgetLeft *lDateAssigned = new LabeledWidgetLeft("Date Assigned", cDateAssigned);
    LabeledWidgetLeft *lComplete = new LabeledWidgetLeft("Complete", cComplete);
    LabeledWidgetLeft *lExpectedFee = new LabeledWidgetLeft("Expected Fee $", cExpectedFee);
    LabeledWidgetLeft *lFeePercent = new LabeledWidgetLeft("Fee %", cFeePercent);
    LabeledWidgetLeft *lWorkAttorney = new LabeledWidgetLeft("Work Attorney", cWorkAttorney);
    LabeledWidgetLeft *lParalegal = new LabeledWidgetLeft("Paralegal", cParalegal);
    LabeledWidgetLeft *lTaskType;

    std::shared_ptr<Deadline> mDeadline;

private:
    void Initialize(void);
    viewParalegalsModel *mParalegalsModel = new viewParalegalsModel(this);
};

} // namespace Nutmeg

#endif // TASKPANELFULL_H
