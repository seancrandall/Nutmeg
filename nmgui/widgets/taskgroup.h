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
    TaskGroup(std::shared_ptr<Task> task, QWidget *parent = nullptr);
    TaskGroup(Key taskid, QWidget *parent = nullptr);

protected:
    std::shared_ptr<Task> mTask;
};

} // namespace Nutmeg

#endif // TASKGROUP_H
