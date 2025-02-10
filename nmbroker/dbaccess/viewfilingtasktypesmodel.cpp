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
        mIsLoaded = true;
        IndexLocations();
    }
}

QSqlRecord viewFilingTaskTypesModel::record(Key primaryKey)
{
	//gViewFilingTaskTypes is already globally defined. 
	return Nutmeg::record<viewFilingTaskTypesModel>(primaryKey, gViewFilingTaskTypesModel);
}

} // namespace Nutmeg
