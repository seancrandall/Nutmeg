#ifndef NUTMEG_VIEWAPPOINTMENTOBJECTSMODEL_H
#define NUTMEG_VIEWAPPOINTMENTOBJECTSMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class viewAppointmentObjectsModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewAppointmentObjectsModel(QObject *parent = nullptr);

    ReadOnlyProperty(getAppointmentObject) Key objectForAppointment[];
    ReadOnlyProperty(getObjectAppointments) QList<Key> appointmentsForObject[];
    
    static QSqlRecord fetchRecord(Key primaryKey);

    Key getAppointmentObject(Key appointmentId);
    QList<Key> getObjectAppointments(Key objectId);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWAPPOINTMENTOBJECTSMODEL_H
