#ifndef NUTMEG_APPOINTMENT_H
#define NUTMEG_APPOINTMENT_H

#include "object.h"
#include "structures.h"

#define appointmentTableName "appointment"
namespace Nutmeg
{

typedef enum{
    ExaminerInterviewPatent = 1,
    InventorIinterviewTrademark = 2,
    InventorInterview = 3,
    ClientCall = 4,
    ClientOfficeHours = 5,
    UnspecifiedAppointment = 999
} AppointmentType;

class Appointment : public Nutmeg::Object
{
  public:
    explicit Appointment();
      explicit Appointment(Key apptid);
    explicit Appointment(DateTime apptime, Key taskId);

    Property(getId, SetId) Key AppointmentId;
    Property(getAppointmentTime, SetAppointmentTime) DateTime AppointmentTime;
    Property(getfkAppointmentType, SetfkAppointmentType) Key fkAppointmentType;
    Property(setfkAppointmentType, SetfkAppointmentType) Key type;
    ReadOnlyProperty(getAssociatedObject) Key associatedObject;

    // Overrides
    virtual Key getId(void) const override { return mDat.AppointmentId; }

    bool Update(AppointmentData dat);
    virtual bool SetId(Key newid) override;
    virtual bool Commit(void) override;

    DateTime getAppointmentTime(void) const { return mDat.AppointmentTime; }
    Key getfkAppointmentType(void) const { return mDat.fkAppointmentType; }
    Key getAssociatedObject(void) const;

    bool SetAppointmentTime(DateTime newappt);
    bool SetfkAppointmentType(Key newfk);
    //void setAssociatedObject(Key newObject);

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
