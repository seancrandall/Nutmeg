#include "viewfilingsincompletemodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

viewFilingsIncompleteModel::viewFilingsIncompleteModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewFilingsIncomplete");
    if(select())
    {
        IndexLocations();
    }
}

QSqlRecord viewFilingsIncompleteModel::fetchRecord(Key primaryKey)
{
	//gViewFilingsIncomplete is already globally defined. 
	return Nutmeg::record<viewFilingsIncompleteModel>(primaryKey, gViewFilingsIncompleteModel);
}

} // namespace Nutmeg
