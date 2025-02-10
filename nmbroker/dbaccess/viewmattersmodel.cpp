#include "viewmattersmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

viewMattersModel::viewMattersModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewMatters");
    if(select())
    {
        mIsLoaded = true;
        IndexLocations();
    }
}

QSqlRecord viewMattersModel::record(Key primaryKey)
{
	//gViewMatters is already globally defined. 
	return Nutmeg::record<viewMattersModel>(primaryKey, gViewMattersModel);
}

} // namespace Nutmeg
