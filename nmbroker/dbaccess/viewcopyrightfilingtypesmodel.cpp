#include "viewcopyrightfilingtypesmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

viewCopyrightFilingTypesModel::viewCopyrightFilingTypesModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewCopyrightFilingTypes");
    if(select())
    {
        IndexLocations();
    }
}

QSqlRecord viewCopyrightFilingTypesModel::fetchRecord(Key primaryKey)
{
	//gViewCopyrightFilingTypes is already globally defined. 
	return Nutmeg::record<viewCopyrightFilingTypesModel>(primaryKey, gViewCopyrightFilingTypesModel);
}

} // namespace Nutmeg
