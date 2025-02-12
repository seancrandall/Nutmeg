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
    UnspecifiedAppointment = 9999
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
    ReadOnlyProperty(getAssociatedObject) Key associatedObject;
    ReadOnlyProperty(getTypeString) QString typeString;
    Property(getComplete, setComplete) bool complete;
    Property(getNeedsAgenda, setNeedsAgenda) bool needsAgenda;
    Property(getAgendaSent, setAgendaSent) bool agentdaSent;
    Property(getConfirmed, setConfirmd) bool confirmed;

    // Overrides
    virtual Key getId(void) const override;

    bool Update(AppointmentData dat);
    virtual bool SetId(Key newid) override;
    virtual bool Commit(void) override;

    DateTime getAppointmentTime(void) const;
    Key getfkAppointmentType(void) const;
    Key getAssociatedObject(void) const;
    QString getTypeString(void) const;
    bool getNeedsAgenda(void);
    bool getAgendaSent(void);
    bool getComplete(void) const;
    bool getConfirmed(void);

    bool SetAppointmentTime(DateTime newappt);
    bool SetfkAppointmentType(Key newfk);
    //void setAssociatedObject(Key newObject);
    void setNeedsAgenda(bool val);
    void setAgendaSent(bool val);
    void setComplete(bool val);
    void setConfirmed(bool val);

    void holdAppointmentTime(DateTime newval);
    void holdfkAppointmentType(Key newval);
    void holdComplete(bool done);

  protected:
    bool InitializeAppointment(Key id);

    AppointmentData mDat;
    bool bNeedsAgenda;
    bool bAgendaSent;
    bool bConfirmed;
private:
    static Appointment* GetAppointment(Key id);
};

} // namespace Nutmeg

#endif // NUTMEG_APPOINTMENT_H
