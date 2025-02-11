#include "viewappointmentobjectsmodel.h"
#include "models.h"
#include "qsqlfield.h"
#include "record.h"

namespace Nutmeg {

viewAppointmentObjectsModel::viewAppointmentObjectsModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewAppointmentObjects");
    if(select())
    {
        IndexLocations();
    }
}

QSqlRecord viewAppointmentObjectsModel::fetchRecord(Key primaryKey)
{
	//gViewAppointmentObjects is already globally defined. 
	return Nutmeg::record<viewAppointmentObjectsModel>(primaryKey, gViewAppointmentObjectsModel);
}

Key viewAppointmentObjectsModel::getAppointmentObject(Key appointmentId)
{
    for(int i=0; i < rowCount(); i++){
        QSqlRecord rec = record(i);
        if(rec.field("AppointmentId").value().toUInt() == appointmentId){
            return rec.field("ObjectId").value().toUInt();
        }
    }

    return 0;
}

QList<Key> viewAppointmentObjectsModel::getObjectAppointments(Key objectId)
{
    QList<Key> list = QList<Key>();

    for(int i=0; i < rowCount(); i++){
        QSqlRecord rec = record(i);
        if(rec.field("ObjectId").value().toUInt() == objectId){
            list << rec.field("AppointmentId").value().toUInt();
        }
    }

    return list;
}

} // namespace Nutmeg
