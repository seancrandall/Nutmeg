#include "taskmodel.h"

namespace Nutmeg {

taskModel::taskModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("task");
    if(select())
    {
        mIsLoaded = true;
        IndexLocations();
    }
}

} // namespace Nutmeg
