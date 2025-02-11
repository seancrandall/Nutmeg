#include "appointment.h"

namespace Nutmeg
{

Appointment::Appointment()
    : Nutmeg::Object{} {}

Appointment::Appointment(Key apptid)
    : Nutmeg::Object{apptid}
{
    auto& appointmentCache = getCache<Appointment>();
    if (appointmentCache.contains(apptid)) {  // Check if appointment is already in cache
        Appointment* cachedAppointment = *appointmentCache.object(apptid);
        if (cachedAppointment) {
            *this = *cachedAppointment;  // Copy state if found in cache
            mObjectIsNull = false;
            return;  // Exit constructor early if we've copied from cache
        }
    }
    // If not in cache, proceed with initialization
    InitializeAppointment(apptid);
}

//Static method to get appointment
Appointment* Appointment::GetAppointment(Key id) {
    auto& appointmentCache = getCache<Appointment>();
    if (appointmentCache.contains(id)) {
        return *appointmentCache.object(id);  // Return from cache if available
    }

    // If not in cache, create new Appointment and initialize it
    Appointment* newAppointment = new Appointment(id);
    if (!newAppointment->mObjectIsNull) { // Assuming mObjectIsNull becomes false on successful initialization
        appointmentCache.insert(id, &newAppointment);
        return newAppointment;
    } else {
        delete newAppointment;  // Clean up if initialization failed
        return nullptr;
    }
}

Appointment::Appointment(DateTime apptime, Key taskId) : Nutmeg::Object{}
{
    mDat.AppointmentId = Nutdb::InsertAppointment(apptime, taskId);
    InitializeAppointment(mDat.AppointmentId);
}

bool Appointment::Update(AppointmentData dat)
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
    return Update(mDat);
}

Key Appointment::getAssociatedObject() const
{
    return Nutdb::GetAppointmentObject(mDat.AppointmentId);
}

QString Appointment::getTypeString() const
{
    return Nutdb::GetAppointmentTypeString(mDat.AppointmentId);
}

bool Appointment::getNeedsAgenda()
{
    return getFlag("NeedsAgenda");
}

bool Appointment::getAgendaSent()
{
    return getFlag("AgendaSent");
}

bool Appointment::getComplete() const
{
    return mDat.Complete;
}

bool Appointment::SetAppointmentTime(DateTime newappt)
{
    bool result = WriteDateTime(appointmentTableName, "AppointmentTime", newappt);
    if(result) mDat.AppointmentTime = newappt;
    return result;
}

bool Appointment::SetfkAppointmentType(Key newfk)
{
    bool result = WriteKey(appointmentTableName, "fkAppointmentType", newfk);
    if(result) mDat.fkAppointmentType = newfk;
    return result;
}

void Appointment::setNeedsAgenda(bool val)
{
    SetFlagValue("NeedsAgenda", val);
}

void Appointment::setAgendaSent(bool val)
{
    SetFlagValue("AgendaSent", val);
}

void Appointment::setComplete(bool val)
{
    bool result = WriteBoolean(appointmentTableName, "Complete", val);
    if(result) mDat.Complete = val;
}

bool Appointment::InitializeAppointment(Key id)
{
    mDat = Nutdb::GetAppointment(id);
    if(mDat.AppointmentId == 0)
    {
        return false;
    }

    bNeedsAgenda = getNeedsAgenda();
    bAgendaSent = getAgendaSent();

    return Object::SetId(id);
}

Key Nutmeg::Appointment::getId() const { return mDat.AppointmentId; }

DateTime Nutmeg::Appointment::getAppointmentTime() const { return mDat.AppointmentTime; }

Key Nutmeg::Appointment::getfkAppointmentType() const { return mDat.fkAppointmentType; }

void Nutmeg::Appointment::holdAppointmentTime(DateTime newval) {mDat.AppointmentTime = newval; dirty["AppointmentTime"] = true;}

void Nutmeg::Appointment::holdfkAppointmentType(Key newval) {mDat.fkAppointmentType = newval; dirty["fkAppointmentType"] = true;}

void Appointment::holdComplete(bool done)
{
    mDat.Complete = done;
}

} // namespace Nutmeg
