#include "viewclientsmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

viewClientsModel::viewClientsModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewClients");
    if(select())
    {
        mIsLoaded = true;
        IndexLocations();
    }
}

QSqlRecord viewClientsModel::record(Key primaryKey)
{
	//gViewClients is already globally defined. 
	return Nutmeg::record<viewClientsModel>(primaryKey, gViewClientsModel);
}

} // namespace Nutmeg
