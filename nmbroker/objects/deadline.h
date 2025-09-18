#ifndef NUTMEG_DEADLINE_H
#define NUTMEG_DEADLINE_H

#include <QColor>
#include <QDate>

#include "dbaccess/nutdb.h"
#include "nutmeg.h"
#include "property.h"
#include "structures.h"
#include "cache/deadlinecache.h"

#define deadlineTableName  "deadline"

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
    explicit Deadline(Key newid);
    ~Deadline();
    static Deadline* GetDeadline(Key id);

    Property(getId, SetId) Key DeadlineId;
    Property(getTriggerDate, SetTriggerDate) Date TriggerDate;
    Property(getSoftDeadline, SetSoftDeadline) Date SoftDeadline;
    Property(getHardDeadline, SetHardDeadline) Date HardDeadline;
    Property(getNextDeadline, SetNextDeadline) Date NextDeadline;
    Property(getUpdatePolicy, SetUpdatePolicy) UpdatePolicy updatePolicy;
    ReadOnlyProperty(getColor) QColor color;

    // Getters
    Key getId(void) { return mDat.DeadlineId; }
    Date getTriggerDate(void) { return mDat.TriggerDate; }
    Date getSoftDeadline(void) { return mDat.SoftDeadline; }
    Date getHardDeadline(void) { return mDat.HardDeadline; }
    Date getNextDeadline(void) { return mDat.NextDeadline; }

    QColor getColor(void);
    static const QColor getDateColor(const QDate& referenceDate);

    bool SetId(Key newval);
    bool Commit(void);
    bool Update(DeadlineData newval);

    bool SetTriggerDate(const QDate &newval);
    bool SetSoftDeadline(const QDate &newval);
    bool SetHardDeadline(const QDate &newval);
    bool SetNextDeadline(const Date &newval);

    void holdTriggerDate(const QDate &newval){mDat.TriggerDate = newval;}
    void holdSoftDeadline(const QDate &newval){mDat.SoftDeadline = newval;}
    void holdHardDeadline(const QDate &newval){mDat.HardDeadline = newval;}
    void holdNextDeadline(const QDate &newval){mDat.NextDeadline = newval;}

    // Update policy accessors
    UpdatePolicy getUpdatePolicy(void) { return mUpdatePolicy; }
    void SetUpdatePolicy(UpdatePolicy policy) { mUpdatePolicy = policy; }

  protected:
    DeadlineData mDat;
    UpdatePolicy mUpdatePolicy = Instant;

    bool InitializeDeadline(Key id);
};

//extern DeadlineCache deadlineCache;


} // namespace Nutmeg

#endif // NUTMEG_DEADLINE_H
