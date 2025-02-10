#include "viewtaskclassmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

viewTaskClassModel::viewTaskClassModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewTaskClass");
    if(select())
    {
        mIsLoaded = true;
        IndexLocations();
    }
}

QSqlRecord viewTaskClassModel::record(Key primaryKey)
{
	//gViewTaskClass is already globally defined. 
	return Nutmeg::record<viewTaskClassModel>(primaryKey, gViewTaskClassModel);
}

} // namespace Nutmeg
