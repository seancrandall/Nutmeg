#ifndef TASKDIALOG_H
#define TASKDIALOG_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>

#include "dialog.h"
#include "objects/task.h"

#include "panels/taskpanelfull.h"
#include "panels/matterpanel.h"

namespace Nutmeg {

class TaskDialog : public Nutmeg::Dialog
{
    Q_OBJECT
public:
    TaskDialog(Task *task, QWidget *parent = nullptr);
    TaskDialog(Key taskid, QWidget *parent = nullptr);

public slots:
    virtual void slotScatter(void) override;
    virtual void slotGather(void) override;
    virtual void slotHandleRejected(void) override;
    virtual void slotHandleAccepted(void) override;

protected:
    Task *mTask;
    Matter *mMatter;

    virtual void setupDisplay(void) override;
    virtual void connectSignalsAndSlots(void) override;

    TaskPanelFull *taskPanel;
    MatterPanel *matterPanel;

    QVBoxLayout *grid = new QVBoxLayout();

private:
    void Initialize(void);
};

} // namespace Nutmeg

#endif // TASKDIALOG_H
