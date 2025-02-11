#include "viewtrademarktasktypesmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

viewTrademarkTaskTypesModel::viewTrademarkTaskTypesModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewTrademarkTaskTypes");
    if(select())
    {
        IndexLocations();
    }
}

QSqlRecord viewTrademarkTaskTypesModel::fetchRecord(Key primaryKey)
{
	//gViewTrademarkTaskTypes is already globally defined. 
	return Nutmeg::record<viewTrademarkTaskTypesModel>(primaryKey, gViewTrademarkTaskTypesModel);
}

} // namespace Nutmeg
