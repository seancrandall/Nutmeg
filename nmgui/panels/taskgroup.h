#ifndef TASKGROUP_H
#define TASKGROUP_H

#include <QObject>
#include <QWidget>
#include "frame.h"

#include "nutmeg.h"
#include "property.h"
#include "objects/task.h"

namespace Nutmeg {

class TaskGroup : public Nutmeg::Frame
{
    Q_OBJECT
public:
    TaskGroup(Task* task, QWidget *parent = nullptr);
    TaskGroup(Key taskid, QWidget *parent = nullptr);

protected:
    Task* mTask;
};

} // namespace Nutmeg

#endif // TASKGROUP_H
