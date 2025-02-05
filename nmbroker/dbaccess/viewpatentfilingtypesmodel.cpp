#include "viewpatentfilingtypesmodel.h"

namespace Nutmeg
{

viewPatentFilingTypesModel::viewPatentFilingTypesModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("viewPatentFilingTypes");
    select();
    mIsLoaded = true;
    IndexLocations();
}

} // namespace Nutmeg
