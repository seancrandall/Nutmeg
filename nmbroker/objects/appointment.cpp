#include "appointment.h"
#include "logger.h"
#include <QDateTime>

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
    Logger::LogMessage(QString("[Appointment] ctor(id=%1) initializing").arg(apptid));
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
    Logger::LogMessage(QString("[Appointment] Update requested id=%1 time=%2 fkType=%3 complete=%4")
                       .arg(dat.AppointmentId)
                       .arg(dat.AppointmentTime.toString(Qt::ISODate))
                       .arg(dat.fkAppointmentType)
                       .arg(dat.Complete));
    bool result = Nutdb::UpdateAppointment(dat);
    Logger::LogMessage(QString("[Appointment] Update -> Nutdb::UpdateAppointment returned %1").arg(result));
    if(!result) return false;

    result = Object::Commit();
    Logger::LogMessage(QString("[Appointment] Update -> Object::Commit returned %1").arg(result));
    if(!result) return false;

    const bool reloaded = InitializeAppointment(dat.AppointmentId);
    Logger::LogMessage(QString("[Appointment] Update -> InitializeAppointment(%1) returned %2")
                       .arg(dat.AppointmentId)
                       .arg(reloaded));
    return reloaded;
}

bool Appointment::SetId(Key newid)
{
    return InitializeAppointment(newid);
}

bool Appointment::Commit()
{
    Logger::LogMessage(QString("[Appointment] Commit id=%1").arg(mDat.AppointmentId));
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

bool Appointment::getConfirmed()
{
    return getFlag("Confirmed");
}

bool Appointment::SetAppointmentTime(DateTime newappt)
{
    Logger::LogMessage(QString("[Appointment] SetAppointmentTime id=%1 new=%2")
                       .arg(mDat.AppointmentId)
                       .arg(newappt.toString(Qt::ISODate)));
    bool result = WriteDateTime(appointmentTableName, "AppointmentTime", newappt);
    Logger::LogMessage(QString("[Appointment] SetAppointmentTime -> write %1").arg(result));
    if(result) mDat.AppointmentTime = newappt;
    return result;
}

bool Appointment::SetfkAppointmentType(Key newfk)
{
    Logger::LogMessage(QString("[Appointment] SetfkAppointmentType id=%1 new=%2")
                       .arg(mDat.AppointmentId)
                       .arg(newfk));
    bool result = WriteKey(appointmentTableName, "fkAppointmentType", newfk);
    Logger::LogMessage(QString("[Appointment] SetfkAppointmentType -> write %1").arg(result));
    if(result) mDat.fkAppointmentType = newfk;
    return result;
}

void Appointment::setNeedsAgenda(bool val)
{
    const Key assoc = getAssociatedObject();
    Logger::LogMessage(QString("[Appointment] setNeedsAgenda id=%1 assocObject=%2 val=%3")
                       .arg(mDat.AppointmentId)
                       .arg(assoc)
                       .arg(val));
    bool ok = SetFlagValue("NeedsAgenda", val);
    Logger::LogMessage(QString("[Appointment] setNeedsAgenda -> SetFlagValue result=%1").arg(ok));
}

void Appointment::setAgendaSent(bool val)
{
    const Key assoc = getAssociatedObject();
    Logger::LogMessage(QString("[Appointment] setAgendaSent id=%1 assocObject=%2 val=%3")
                       .arg(mDat.AppointmentId)
                       .arg(assoc)
                       .arg(val));
    bool ok = SetFlagValue("AgendaSent", val);
    Logger::LogMessage(QString("[Appointment] setAgendaSent -> SetFlagValue result=%1").arg(ok));
}

void Appointment::setComplete(bool val)
{
    Logger::LogMessage(QString("[Appointment] setComplete id=%1 val=%2")
                       .arg(mDat.AppointmentId)
                       .arg(val));
    bool result = WriteBoolean(appointmentTableName, "Complete", val);
    Logger::LogMessage(QString("[Appointment] setComplete -> write %1").arg(result));
    if(result) mDat.Complete = val;
}

void Appointment::setConfirmed(bool val)
{
    const Key assoc = getAssociatedObject();
    Logger::LogMessage(QString("[Appointment] setConfirmed id=%1 assocObject=%2 val=%3")
                       .arg(mDat.AppointmentId)
                       .arg(assoc)
                       .arg(val));
    bool ok = SetFlagValue("Confirmed", val);
    Logger::LogMessage(QString("[Appointment] setConfirmed -> SetFlagValue result=%1").arg(ok));
}

bool Appointment::InitializeAppointment(Key id)
{
    Logger::LogMessage(QString("[Appointment] InitializeAppointment(%1)").arg(id));
    mDat = Nutdb::GetAppointment(id);
    if(mDat.AppointmentId == 0)
    {
        Logger::LogMessage(QString("[Appointment] InitializeAppointment: not found"));
        return false;
    }

    bNeedsAgenda = getNeedsAgenda();
    bAgendaSent = getAgendaSent();
    bConfirmed = getConfirmed();
    Logger::LogMessage(QString("[Appointment] Initialized id=%1 time=%2 fkType=%3 complete=%4 flags{NeedsAgenda=%5, AgendaSent=%6, Confirmed=%7} assocObject=%8")
                       .arg(mDat.AppointmentId)
                       .arg(mDat.AppointmentTime.toString(Qt::ISODate))
                       .arg(mDat.fkAppointmentType)
                       .arg(mDat.Complete)
                       .arg(bNeedsAgenda)
                       .arg(bAgendaSent)
                       .arg(bConfirmed)
                       .arg(getAssociatedObject()));

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
