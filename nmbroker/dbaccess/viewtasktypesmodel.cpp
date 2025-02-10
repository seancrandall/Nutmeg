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
        mIsLoaded = true;
        IndexLocations();
    }
}

QSqlRecord viewTaskTypesModel::record(Key primaryKey)
{
	//gViewTaskTypes is already globally defined. 
	return Nutmeg::record<viewTaskTypesModel>(primaryKey, gViewTaskTypesModel);
}

} // namespace Nutmeg
