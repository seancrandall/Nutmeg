#include "appointment.h"

namespace Nutmeg
{

Appointment::Appointment()
    : Nutmeg::Object{} {}

Appointment::Appointment(Key apptid)
    : Nutmeg::Object{apptid}
{
    auto& appointmentCache = getCache<Appointment>();
    Appointment* cachedAppointment = cache<Appointment>::getObjectFromCache(apptid, &Appointment::GetAppointment, appointmentCache);
    if (cachedAppointment) {
        // If we find the Appointment in cache, copy its state
        *this = *cachedAppointment;
        mObjectIsNull = false; // Assuming this is inherited from Object
    } else {
        // If not in cache, proceed with initialization
        InitializeAppointment(apptid);
    }
}

// Static method to fetch an Appointment from the database if not in cache
Appointment* Appointment::GetAppointment(Key id) {
    Appointment* appointment = new Appointment(id); // This will call the constructor again, but now with cache check
    if (appointment->mObjectIsNull) { // Assuming this is inherited from Object
        delete appointment; // Clean up if initialization failed
        return nullptr;
    }
    return appointment;
}
Appointment::Appointment(DateTime apptime, Key taskId) : Nutmeg::Object{}
{
    mDat.AppointmentId = Nutdb::InsertAppointment(apptime, taskId);
    InitializeAppointment(mDat.AppointmentId);
}

bool Appointment::slotUpdate(AppointmentData dat)
{
    bool result = Nutdb::UpdateAppointment(dat);
    if(!result) return false;

    result = Object::Commit();
    if(!result) return false;

    return InitializeAppointment(dat.AppointmentId);
}

bool Appointment::SetId(Key newid)
{
    return InitializeAppointment(newid);
}

bool Appointment::Commit()
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
    return Object::SetId(id);
}

} // namespace Nutmeg
