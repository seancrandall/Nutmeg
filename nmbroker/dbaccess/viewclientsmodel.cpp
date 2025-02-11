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
        IndexLocations();
    }
}

QSqlRecord viewClientsModel::fetchRecord(Key primaryKey)
{
	//gViewClients is already globally defined. 
	return Nutmeg::record<viewClientsModel>(primaryKey, gViewClientsModel);
}

} // namespace Nutmeg
