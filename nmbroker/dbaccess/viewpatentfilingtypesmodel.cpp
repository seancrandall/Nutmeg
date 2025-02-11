#include "viewpatentfilingtypesmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

viewPatentFilingTypesModel::viewPatentFilingTypesModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewPatentFilingTypes");
    if(select())
    {
        IndexLocations();
    }
}

QSqlRecord viewPatentFilingTypesModel::fetchRecord(Key primaryKey)
{
	//gViewPatentFilingTypes is already globally defined. 
	return Nutmeg::record<viewPatentFilingTypesModel>(primaryKey, gViewPatentFilingTypesModel);
}

} // namespace Nutmeg
