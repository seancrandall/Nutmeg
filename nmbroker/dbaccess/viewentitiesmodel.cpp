#include "viewentitiesmodel.h"

namespace Nutmeg
{

viewEntitiesModel::viewEntitiesModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("viewEntities");
    select();
    mIsLoaded = true;
    IndexLocations();
}

} // namespace Nutmeg
