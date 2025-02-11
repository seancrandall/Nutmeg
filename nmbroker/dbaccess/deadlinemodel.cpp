#include "deadlinemodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

deadlineModel::deadlineModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("deadline");
    if(select())
    {
        IndexLocations();
    }
}

QSqlRecord deadlineModel::fetchRecord(Key primaryKey)
{
	//gDeadline is already globally defined. 
	return Nutmeg::record<deadlineModel>(primaryKey, gDeadlineModel);
}

} // namespace Nutmeg
