#include "responsesdashboardmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

responsesDashboardModel::responsesDashboardModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("responsesDashboard");
    if(select())
    {
        mIsLoaded = true;
        IndexLocations();
    }
}

QSqlRecord responsesDashboardModel::record(Key primaryKey)
{
	//gResponsesDashboard is already globally defined. 
	return Nutmeg::record<responsesDashboardModel>(primaryKey, gResponsesDashboardModel);
}

} // namespace Nutmeg
