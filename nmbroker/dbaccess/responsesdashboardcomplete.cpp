#include "responsesdashboardcomplete.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

ResponsesDashboardComplete::ResponsesDashboardComplete(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("responsesDashboardComplete");
    if (select())
    {
        IndexLocations();
    }
}

//QSqlRecord ResponsesDashboardComplete::record(Key primaryKey)
//{
        //return Nutmeg::record<ResponsesDashboardComplete>(primaryKey, gResponsesDashboardComplete);
    //return this->QSqlRelationalTableModel::record();
//}

} // namespace Nutmeg
