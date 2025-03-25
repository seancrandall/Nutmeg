#include "viewfilingtasktypesmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

viewFilingTaskTypesModel::viewFilingTaskTypesModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewFilingTaskTypes");
    if(select())
    {
        IndexLocations();
    }
}

QSqlRecord viewFilingTaskTypesModel::fetchRecord(Key primaryKey)
{
	//gViewFilingTaskTypes is already globally defined. 
	return Nutmeg::record<viewFilingTaskTypesModel>(primaryKey, gViewFilingTaskTypesModel);
}

} // namespace Nutmeg
