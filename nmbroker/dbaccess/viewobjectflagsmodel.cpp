#include "viewobjectflagsmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

viewObjectFlagsModel::viewObjectFlagsModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewObjectFlags");
    if(select())
    {
        mIsLoaded = true;
        IndexLocations();
    }
}

QSqlRecord viewObjectFlagsModel::record(Key primaryKey)
{
	//gViewObjectFlags is already globally defined. 
	return Nutmeg::record<viewObjectFlagsModel>(primaryKey, gViewObjectFlagsModel);
}

} // namespace Nutmeg
