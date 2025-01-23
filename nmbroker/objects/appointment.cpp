#include "appointment.h"

namespace Nutmeg
{

Appointment::Appointment(QObject *parent) : Nutmeg::Object{parent} {}

Appointment::Appointment(Key apptid, QObject *parent) : Nutmeg::Object{apptid, parent}
{
    InitializeAppointment(apptid);
}

Appointment::Appointment(DateTime apptime, Key taskId, QObject *parent) : Nutmeg::Object{parent}
{
    mDat.AppointmentId = Nutdb::InsertAppointment(apptime, taskId);
    InitializeAppointment(mDat.AppointmentId);
}

bool Appointment::slotUpdate(AppointmentData dat)
{
    bool result = Nutdb::UpdateAppointment(dat);
    if(!result) return false;

    result = Object::slotCommit();
    if(!result) return false;

    return InitializeAppointment(dat.AppointmentId);
}

bool Appointment::slotSetId(Key newid)
{
    return InitializeAppointment(newid);
}

bool Appointment::slotCommit()
{
    return slotUpdate(mDat);
}

bool Appointment::slotSetAppointmentTime(DateTime newappt)
{
    bool result = WriteDateTime(appointmentTableName, "AppointmentTime", newappt);
    if(result) mDat.AppointmentTime = newappt;
    return result;
}

bool Appointment::slotSetfkAppointmentType(Key newfk)
{
    bool result = WriteKey(appointmentTableName, "fkAppointmentType", newfk);
    if(result) mDat.fkAppointmentType = newfk;
    return result;
}

bool Appointment::InitializeAppointment(Key id)
{
    mDat = Nutdb::GetAppointment(id);
    if(mDat.AppointmentId == 0)
    {
        return false;
    }
    return Object::slotSetId(id);
}

} // namespace Nutmeg
