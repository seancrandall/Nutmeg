#include "viewtrademarkmattersmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

viewTrademarkMattersModel::viewTrademarkMattersModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewTrademarkMatters");
    if(select())
    {
        IndexLocations();
    }
}

QSqlRecord viewTrademarkMattersModel::fetchRecord(Key primaryKey)
{
	//gViewTrademarkMatters is already globally defined. 
	return Nutmeg::record<viewTrademarkMattersModel>(primaryKey, gViewTrademarkMattersModel);
}

} // namespace Nutmeg
