#include "viewtasktypesmodel.h"

namespace Nutmeg
{

viewTaskTypesModel::viewTaskTypesModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("viewTaskTypes");
    select();
    mIsLoaded = true;
    IndexLocations();
}

} // namespace Nutmeg
