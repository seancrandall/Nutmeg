#include "appointmenttypemodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

appointmentTypeModel::appointmentTypeModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("appointmentType");
    if(select())
    {
        IndexLocations();
    }
}

QSqlRecord appointmentTypeModel::fetchRecord(Key primaryKey)
{
	//gAppointmentType is already globally defined. 
	return Nutmeg::record<appointmentTypeModel>(primaryKey, gAppointmentTypeModel);
}

} // namespace Nutmeg
