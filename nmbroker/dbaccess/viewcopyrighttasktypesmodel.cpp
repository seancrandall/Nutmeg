#include "viewcopyrighttasktypesmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

viewCopyrightTaskTypesModel::viewCopyrightTaskTypesModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewCopyrightTaskTypes");
    if(select())
    {
        IndexLocations();
    }
}

QSqlRecord viewCopyrightTaskTypesModel::fetchRecord(Key primaryKey)
{
	//gViewCopyrightTaskTypes is already globally defined. 
	return Nutmeg::record<viewCopyrightTaskTypesModel>(primaryKey, gViewCopyrightTaskTypesModel);
}

} // namespace Nutmeg
