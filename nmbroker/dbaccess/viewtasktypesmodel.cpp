#include "viewtasktypesmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

viewTaskTypesModel::viewTaskTypesModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewTaskTypes");
    if(select())
    {
        IndexLocations();
    }
}

QSqlRecord viewTaskTypesModel::fetchRecord(Key primaryKey)
{
	//gViewTaskTypes is already globally defined. 
	return Nutmeg::record<viewTaskTypesModel>(primaryKey, gViewTaskTypesModel);
}

} // namespace Nutmeg
