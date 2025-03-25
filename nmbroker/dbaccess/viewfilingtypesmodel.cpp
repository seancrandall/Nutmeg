#include "viewfilingtypesmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

viewFilingTypesModel::viewFilingTypesModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewFilingTypes");
    if(select())
    {
        IndexLocations();
    }
}

QSqlRecord viewFilingTypesModel::fetchRecord(Key primaryKey)
{
	//gViewFilingTypes is already globally defined. 
	return Nutmeg::record<viewFilingTypesModel>(primaryKey, gViewFilingTypesModel);
}

} // namespace Nutmeg
