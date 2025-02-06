#include "viewtasksmodel.h"

namespace Nutmeg {

viewTasksModel::viewTasksModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewTasks");
    if(select())
    {
        mIsLoaded = true;
        IndexLocations();
    }
}

} // namespace Nutmeg
