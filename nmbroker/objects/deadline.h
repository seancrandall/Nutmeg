#ifndef NUTMEG_DEADLINE_H
#define NUTMEG_DEADLINE_H

#include <QColor>
#include <QDate>

#include "dbaccess/nutdb.h"
#include "nutmeg.h"
#include "property.h"
#include "structures.h"

namespace Nutmeg
{

// Color Definitions
#define OverdueItemColor 0xB22222  // FireBrick
#define OneDayItemColor 0xFF8C00   // DarkOrange
#define OneWeekItemColor 0xFFD700  // Gold
#define TwoWeekItemColor 0x90EE90  // LightGreen
#define OneMonthItemColor 0x93FFE8 // LightAquamarine

class Deadline
{

    enum UpdatePolicy
    {
        OnCommit = 0,
        Instant = 1
    };

  public:
    Deadline();
    Deadline(Key newid);

    Property(getId, slotSetId) Key DeadlineId;
    Property(getTriggerDate, slotSetTriggerDate) Date TriggerDate;
    Property(getSoftDeadline, slotSetSoftDeadline) Date SoftDeadline;
    Property(getHardDeadline, slotSetHardDeadline) Date HardDeadline;
    Property(getNextDeadline, slotSetNextDeadline) Date NextDeadline;
    Property(getUpdatePolicy, slotSetUpatePolicy) UpdatePolicy updatePolicy;
    ReadOnlyProperty(getColor) QColor color;

    // Getters
    Key getId(void) { return mDat.DeadlineId; }
    Date getTriggerDate(void) { return mDat.TriggerDate; }
    Date getSoftDeadline(void) { return mDat.SoftDeadline; }
    Date getHardDeadline(void) { return mDat.HardDeadline; }
    Date getNextDeadline(void) { return mDat.NextDeadline; }

    QColor getColor(void);
    static const QColor getDateColor(const QDate& referenceDate);

  public slots:
    bool slotSetId(Key newval);
    bool slotCommit(void);
    bool slotUpdate(DeadlineData newval);

    bool slotSetTriggerDate(const QDate &newval);
    bool slotSetSoftDeadline(const QDate &newval);
    bool slotSetHardDeadline(const QDate &newval);
    bool slotSetNextDeadline(const Date &newval);

    void holdTriggerDate(const QDate &newval){mDat.TriggerDate = newval;}
    void holdSoftDeadline(const QDate &newval){mDat.SoftDeadline = newval;}
    void holdHardDeadline(const QDate &newval){mDat.HardDeadline = newval;}
    void holdNextDeadline(const QDate &newval){mDat.NextDeadline = newval;}

  protected:
    DeadlineData mDat;
    UpdatePolicy mUpdatePolicy = Instant;

    bool InitializeDeadline(Key id);
    const QString deadlineTableName = "deadline";
};

} // namespace Nutmeg

#endif // NUTMEG_DEADLINE_H
