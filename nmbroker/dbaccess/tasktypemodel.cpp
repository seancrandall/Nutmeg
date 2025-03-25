#include "tasktypemodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

taskTypeModel::taskTypeModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("taskType");
    if(select())
    {
        IndexLocations();
    }
}

QSqlRecord taskTypeModel::fetchRecord(Key primaryKey)
{
	//gTaskType is already globally defined. 
	return Nutmeg::record<taskTypeModel>(primaryKey, gTaskTypeModel);
}

} // namespace Nutmeg
