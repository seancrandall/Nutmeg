#include "viewupcomingappointmentsmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

viewUpcomingAppointmentsModel::viewUpcomingAppointmentsModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewUpcomingAppointments");
    if(select())
    {
        IndexLocations();
    }
}

QSqlRecord viewUpcomingAppointmentsModel::fetchRecord(Key primaryKey)
{
	//gViewUpcomingAppointments is already globally defined. 
	return Nutmeg::record<viewUpcomingAppointmentsModel>(primaryKey, gViewUpcomingAppointmentsModel);
}

} // namespace Nutmeg
