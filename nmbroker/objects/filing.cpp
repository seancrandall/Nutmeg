#include "filing.h"

namespace Nutmeg
{

Filing::Filing() : Nutmeg::Task{} {}

Filing::Filing(Key id) : Nutmeg::Task(id)
{
    auto& filingCache = Nutmeg::getCache<Filing>();
    Filing* cachedFiling = Nutmeg::cache<Filing>::getObjectFromCache(id, &Filing::GetFiling, filingCache);
    if (cachedFiling) {
        // If we find the Filing in cache, copy its state
        *this = *cachedFiling;
        mObjectIsNull = false; // Assuming this is inherited from Object via Task
    } else {
        // If not in cache, proceed with initialization
        InitializeFiling(id);
    }
}

// Static method to fetch a Filing from the database if not in cache
Filing* Filing::GetFiling(Key id) {
    Filing* filing = new Filing(id); // This will call the constructor again, but now with cache check
    if (filing->mObjectIsNull) { // Assuming this is inherited from Object via Task
        delete filing; // Clean up if initialization failed
        return nullptr;
    }
    return filing;
}
bool Filing::slotUpdate(FilingData dat)
{
    bool result = Nutdb::UpdateFiling(dat);
    if(!result) return false;

    result = Task::Commit();
    if(!result) return false;

    return InitializeFiling(dat.FilingId);
}

bool Filing::SetId(Key newval)
{
    return InitializeFiling(newval);
}

bool Filing::Commit()
{
    return slotUpdate(mDat);
}

bool Filing::setfkFilingStatus(Key newval)
{
    bool result = WriteKey(filingTableName, "fkFilingStatus", newval);
    if(result) mDat.fkFilingStatus = newval;
    return result;
}

bool Filing::setfkAsFiledDocument(Key newval)
{
    bool result = WriteKey(filingTableName, "fkAsFiledDocument", newval);
    if(result) mDat.fkAsFiledDocument = newval;
    return result;
}

bool Filing::InitializeFiling(Key id)
{
    if(id == 0)
    {
        mDat = FilingData();
        return InitializeTask(0);
    }

    mDat = Nutdb::GetFiling(id);
    if(mDat.FilingId == 0) return InitializeFiling(0);

    return Task::InitializeTask(id);
}

} // namespace Nutmeg
