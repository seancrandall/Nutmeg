#include "viewresponsesincompletemodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

viewResponsesIncompleteModel::viewResponsesIncompleteModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewResponsesIncomplete");
    if(select())
    {
        IndexLocations();
    }
}

QSqlRecord viewResponsesIncompleteModel::fetchRecord(Key primaryKey)
{
	//gViewResponsesIncomplete is already globally defined. 
	return Nutmeg::record<viewResponsesIncompleteModel>(primaryKey, gViewResponsesIncompleteModel);
}

} // namespace Nutmeg
