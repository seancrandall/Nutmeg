#include "filing.h"

namespace Nutmeg
{

Filing::Filing(QObject *parent) : Nutmeg::Task{parent} {}

Filing::Filing(Key id, QObject *parent) : Nutmeg::Task(id, parent)
{
    InitializeFiling(id);
}

bool Filing::slotUpdate(FilingData dat)
{
    bool result = Nutdb::UpdateFiling(dat);
    if(!result) return false;

    result = Task::slotCommit();
    if(!result) return false;

    return InitializeFiling(dat.FilingId);
}

bool Filing::slotSetId(Key newval)
{
    return InitializeFiling(newval);
}

bool Filing::slotCommit()
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
