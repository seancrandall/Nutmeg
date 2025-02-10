#include "viewfilingsmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

viewFilingsModel::viewFilingsModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewFilings");
    if(select())
    {
        mIsLoaded = true;
        IndexLocations();
    }
}

QSqlRecord viewFilingsModel::record(Key primaryKey)
{
	//gViewFilings is already globally defined. 
	return Nutmeg::record<viewFilingsModel>(primaryKey, gViewFilingsModel);
}

} // namespace Nutmeg
