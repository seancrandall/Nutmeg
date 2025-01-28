#include "taskgroup.h"

namespace Nutmeg {

TaskGroup::TaskGroup(Task* task, QWidget *parent)
    : Frame(parent)
    , mTask(task)
{}

TaskGroup::TaskGroup(Key taskid, QWidget *parent)
    : Frame(parent)
    , mTask(new Task(taskid, this))
{
}

} // namespace Nutmeg
