#ifndef NUTMEG_APPOINTMENT_H
#define NUTMEG_APPOINTMENT_H

#include "object.h"
#include "structures.h"
#include <QObject>

namespace Nutmeg
{

class Appointment : public Nutmeg::Object
{
    Q_OBJECT
  public:
    explicit Appointment(QObject *parent = nullptr);
    explicit Appointment(Key apptid, QObject *parent = nullptr);
    explicit Appointment(DateTime apptime, Key taskId, QObject *parent = nullptr);

    Property(getId, slotSetId) Key AppointmentId;
    Property(getAppointmentTime, slotSetAppointmentTime) DateTime AppointmentTime;
    Property(getfkAppointmentType, slotSetfkAppointmentType) Key fkAppointmentType;

    // Overrides
    virtual Key getId(void) override { return mDat.AppointmentId; }

    bool slotUpdate(AppointmentData dat);
    virtual bool slotSetId(Key newid) override;
    virtual bool slotCommit(void) override;

    DateTime getAppointmentTime(void) { return mDat.AppointmentTime; };
    Key getfkAppointmentType(void) { return mDat.fkAppointmentType; }

  public slots:
    bool slotSetAppointmentTime(DateTime newappt);
    bool slotSetfkAppointmentType(Key newfk);

    void holdAppointmentTime(DateTime newval) {mDat.AppointmentTime = newval; dirty["AppointmentTime"] = true;}
    void holdfkAppointmentType(Key newval) {mDat.fkAppointmentType = newval; dirty["fkAppointmentType"] = true;}

  protected:
    bool InitializeAppointment(Key id);

    AppointmentData mDat;
    const QString appointmentTableName = "appointment";
};

} // namespace Nutmeg

#endif // NUTMEG_APPOINTMENT_H
