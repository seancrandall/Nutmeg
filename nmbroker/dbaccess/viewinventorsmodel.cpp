#include "viewinventorsmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

viewInventorsModel::viewInventorsModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewInventors");
    if(select())
    {
        mIsLoaded = true;
        IndexLocations();
    }
}

QSqlRecord viewInventorsModel::record(Key primaryKey)
{
	//gViewInventors is already globally defined. 
	return Nutmeg::record<viewInventorsModel>(primaryKey, gViewInventorsModel);
}

} // namespace Nutmeg
