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
        mIsLoaded = true;
        IndexLocations();
    }
}

QSqlRecord viewTrademarkTaskTypesModel::record(Key primaryKey)
{
	//gViewTrademarkTaskTypes is already globally defined. 
	return Nutmeg::record<viewTrademarkTaskTypesModel>(primaryKey, gViewTrademarkTaskTypesModel);
}

} // namespace Nutmeg
