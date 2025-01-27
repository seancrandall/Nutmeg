#include "taskgroup.h"

namespace Nutmeg {

TaskGroup::TaskGroup(std::shared_ptr<Task> task, QWidget *parent)
    : Frame(parent)
    , mTask(task)
{}

TaskGroup::TaskGroup(Key taskid, QWidget *parent)
    : Frame(parent)
    , mTask(std::make_shared<Task>(taskid))
{
}

} // namespace Nutmeg
