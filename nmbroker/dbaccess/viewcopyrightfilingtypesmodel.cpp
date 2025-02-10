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
        mIsLoaded = true;
        IndexLocations();
    }
}

QSqlRecord viewCopyrightFilingTypesModel::record(Key primaryKey)
{
	//gViewCopyrightFilingTypes is already globally defined. 
	return Nutmeg::record<viewCopyrightFilingTypesModel>(primaryKey, gViewCopyrightFilingTypesModel);
}

} // namespace Nutmeg
