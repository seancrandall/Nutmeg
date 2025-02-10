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
        mIsLoaded = true;
        IndexLocations();
    }
}

QSqlRecord taskTypeModel::record(Key primaryKey)
{
	//gTaskType is already globally defined. 
	return Nutmeg::record<taskTypeModel>(primaryKey, gTaskTypeModel);
}

} // namespace Nutmeg
