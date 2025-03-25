#include "objectmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

objectModel::objectModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("object");
    if(select())
    {
        IndexLocations();
    }
}

QSqlRecord objectModel::fetchRecord(Key primaryKey)
{
	//gObject is already globally defined. 
	return Nutmeg::record<objectModel>(primaryKey, gObjectModel);
}

} // namespace Nutmeg
