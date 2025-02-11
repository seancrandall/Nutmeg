#include "viewentitiesmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

viewEntitiesModel::viewEntitiesModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewEntities");
    if(select())
    {
        IndexLocations();
    }
}

QSqlRecord viewEntitiesModel::fetchRecord(Key primaryKey)
{
	//gViewEntities is already globally defined. 
	return Nutmeg::record<viewEntitiesModel>(primaryKey, gViewEntitiesModel);
}

} // namespace Nutmeg
