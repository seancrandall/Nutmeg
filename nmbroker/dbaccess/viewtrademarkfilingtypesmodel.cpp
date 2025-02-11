#include "viewtrademarkfilingtypesmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

viewTrademarkFilingTypesModel::viewTrademarkFilingTypesModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewTrademarkFilingTypes");
    if(select())
    {
        IndexLocations();
    }
}

QSqlRecord viewTrademarkFilingTypesModel::fetchRecord(Key primaryKey)
{
	//gViewTrademarkFilingTypes is already globally defined. 
	return Nutmeg::record<viewTrademarkFilingTypesModel>(primaryKey, gViewTrademarkFilingTypesModel);
}

} // namespace Nutmeg
