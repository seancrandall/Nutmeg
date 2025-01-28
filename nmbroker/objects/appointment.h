#ifndef NUTMEG_APPOINTMENT_H
#define NUTMEG_APPOINTMENT_H

#include "object.h"
#include "structures.h"

#define appointmentTableName "appointment"
namespace Nutmeg
{

class Appointment : public Nutmeg::Object
{
  public:
    explicit Appointment();
      explicit Appointment(Key apptid);
    explicit Appointment(DateTime apptime, Key taskId);

    Property(getId, slotSetId) Key AppointmentId;
    Property(getAppointmentTime, slotSetAppointmentTime) DateTime AppointmentTime;
    Property(getfkAppointmentType, slotSetfkAppointmentType) Key fkAppointmentType;

    // Overrides
    virtual Key getId(void) override { return mDat.AppointmentId; }

    bool slotUpdate(AppointmentData dat);
    virtual bool SetId(Key newid) override;
    virtual bool Commit(void) override;

    DateTime getAppointmentTime(void) { return mDat.AppointmentTime; }
    Key getfkAppointmentType(void) { return mDat.fkAppointmentType; }

    bool slotSetAppointmentTime(DateTime newappt);
    bool slotSetfkAppointmentType(Key newfk);

    void holdAppointmentTime(DateTime newval) {mDat.AppointmentTime = newval; dirty["AppointmentTime"] = true;}
    void holdfkAppointmentType(Key newval) {mDat.fkAppointmentType = newval; dirty["fkAppointmentType"] = true;}

  protected:
    bool InitializeAppointment(Key id);

    AppointmentData mDat;
private:
    static Appointment* GetAppointment(Key id);
};

} // namespace Nutmeg

#endif // NUTMEG_APPOINTMENT_H
