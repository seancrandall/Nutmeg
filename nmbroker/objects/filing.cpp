#include "filing.h"

namespace Nutmeg
{

Filing::Filing() : Nutmeg::Task{} {}

Filing::Filing(Key id) : Nutmeg::Task(id)
{
    auto& filingCache = Nutmeg::getCache<Filing>();
    if (filingCache.contains(id)) {  // Check if Filing is already in cache
        Filing* cachedFiling = *filingCache.object(id);
        if (cachedFiling) {
            *this = *cachedFiling;  // Copy state if found in cache
            mObjectIsNull = false;
            return;  // Exit constructor early if we've copied from cache
        }
    }
    // If not in cache, proceed with initialization
    InitializeFiling(id);
}

Filing* Filing::GetFiling(Key id) {
    auto& filingCache = Nutmeg::getCache<Filing>();
    if (filingCache.contains(id)) {
        return *filingCache.object(id);  // Return from cache if available
    }

    // If not in cache, create new Filing and initialize it
    Filing* newFiling = new Filing(id);
    if (!newFiling->mObjectIsNull) { // Assuming mObjectIsNull becomes false on successful initialization
        filingCache.insert(id, &newFiling);
        return newFiling;
    } else {
        delete newFiling;  // Clean up if initialization failed
        return nullptr;
    }
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
