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
        IndexLocations();
    }
}

QSqlRecord viewTasksModel::fetchRecord(Key primaryKey)
{
	//gViewTasks is already globally defined. 
	return Nutmeg::record<viewTasksModel>(primaryKey, gViewTasksModel);
}

} // namespace Nutmeg
