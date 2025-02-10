#include "viewtasksmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

viewTasksModel::viewTasksModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewTasks");
    if(select())
    {
        mIsLoaded = true;
        IndexLocations();
    }
}

QSqlRecord viewTasksModel::record(Key primaryKey)
{
	//gViewTasks is already globally defined. 
	return Nutmeg::record<viewTasksModel>(primaryKey, gViewTasksModel);
}

} // namespace Nutmeg
