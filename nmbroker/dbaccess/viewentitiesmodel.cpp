#include "viewentitiesmodel.h"

namespace Nutmeg
{

viewEntitiesModel::viewEntitiesModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("viewEntities");
    select();
}

} // namespace Nutmeg
