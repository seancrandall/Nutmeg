#include "deadline.h"

namespace Nutmeg
{

Deadline::Deadline()
{
    InitializeDeadline(0);
}

Deadline::Deadline(Key newid)
{
    if (deadlineCache.contains(newid)) {  // Check if the deadline is already in cache
        Deadline* cachedDeadline = *deadlineCache.object(newid);
        if (cachedDeadline) {
            *this = *cachedDeadline;
            return;
        }
    }
    // If not in cache, proceed with initialization
    InitializeDeadline(newid);
}

Deadline* Deadline::GetDeadline(Key id) {
    if (deadlineCache.contains(id)) {  // Check cache first
        return *deadlineCache.object(id);
    }

    Deadline* newDeadline = new Deadline(id);
    if (newDeadline->mDat.DeadlineId == 0) { // Initialization failed
        delete newDeadline;
        return nullptr;
    }

    // Insert into cache only after successful initialization
    Deadline** tempPtr = new Deadline*[1];
    tempPtr[0] = newDeadline;
    deadlineCache.insert(id, tempPtr);
    delete[] tempPtr;  // Clean up the temporary array

    return newDeadline;
}

Deadline::~Deadline()
{

}

QColor Deadline::getColor()
{
    return getDateColor(NextDeadline);
}

const QColor Deadline::getDateColor(const QDate &referenceDate)
{
    if (referenceDate == QDate())
    {
        return QColor(Qt::white);
    }

    QDate today = QDate::currentDate();

    int daysDifference = today.daysTo(referenceDate);

    if (daysDifference < 0)
    {
        return QColor(OverdueItemColor);
    }
    else if (daysDifference <= 2)
    {
        return QColor(OneDayItemColor);
    }
    else if (daysDifference <= 7)
    {
        return QColor(OneWeekItemColor);
    }
    else if (daysDifference <= 14)
    {
        return QColor(TwoWeekItemColor);
    }
    else if (daysDifference <= 30)
    {
        return QColor(OneMonthItemColor);
    }
    else
    {
        return QColor(Qt::white);
    }
}

bool Deadline::SetId(Key newval)
{
    return InitializeDeadline(newval);
}

bool Deadline::Commit()
{
    return Update(mDat);
}

bool Deadline::Update(DeadlineData newval)
{
    bool result = Nutdb::UpdateDeadline(newval);
    if(!result) return false;

    return InitializeDeadline(newval.DeadlineId);
}

bool Deadline::SetTriggerDate(const QDate &newval)
{
    QString stringval = newval.toString("yyyy-MM-dd");
    bool result = Nutdb::UpdateField(deadlineTableName, "TriggerDate", mDat.DeadlineId, stringval);
    if(result) mDat.TriggerDate = newval;
    return result;
}

bool Deadline::SetSoftDeadline(const QDate &newval)
{
    QString stringval = newval.toString("yyyy-MM-dd");
    bool result = Nutdb::UpdateField(deadlineTableName, "SoftDeadline", mDat.DeadlineId, stringval);
    if(result) mDat.SoftDeadline = newval;
    return result;
}

bool Deadline::SetHardDeadline(const QDate &newval)
{
    QString stringval = newval.toString("yyyy-MM-dd");
    bool result = Nutdb::UpdateField(deadlineTableName, "HardDeadline", mDat.DeadlineId, stringval);
    if(result) mDat.HardDeadline = newval;
    return result;
}

bool Deadline::SetNextDeadline(const QDate &newval)
{
    QString stringval = newval.toString("yyyy-MM-dd");
    bool result = Nutdb::UpdateField(deadlineTableName, "NextDeadline", mDat.DeadlineId, stringval);
    if(result) mDat.NextDeadline = newval;
    return result;
}

bool Deadline::InitializeDeadline(Key id)
{
    if(id == 0)
    {
        mDat = DeadlineData();
        return false;
    }

    mDat = Nutdb::GetDeadline(id);
    if(mDat.DeadlineId == 0) return InitializeDeadline(0);

    return true;
}

} // namespace Nutmeg
