#include "taskmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

taskModel::taskModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("task");
    if(select())
    {
        mIsLoaded = true;
        IndexLocations();
    }
}

QSqlRecord taskModel::record(Key primaryKey)
{
	//gTask is already globally defined. 
	return Nutmeg::record<taskModel>(primaryKey, gTaskModel);
}

} // namespace Nutmeg
