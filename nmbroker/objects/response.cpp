#include "response.h"

namespace Nutmeg
{

Response::Response() : Nutmeg::Task{}
{
    mObjectIsNull = true;
}

Response::Response(Key newid) : Nutmeg::Task(newid)
{
    auto& responseCache = getCache<Response>();
    if (responseCache.contains(newid)) {  // Check if Response is already in cache
        Response* cachedResponse = *responseCache.object(newid);
        if (cachedResponse) {
            *this = *cachedResponse;  // Copy state if found in cache
            mObjectIsNull = false;
            return;  // Exit constructor early if we've copied from cache
        }
    }
    // If not in cache, proceed with initialization
    InitializeResponse(newid);
}

Response* Response::GetResponse(Key id) {
    auto& responseCache = getCache<Response>();
    if (responseCache.contains(id)) {
        return *responseCache.object(id);  // Return from cache if available
    }

    // If not in cache, create new Response and initialize it
    Response* newResponse = new Response(id);
    if (!newResponse->mObjectIsNull) { // Assuming mObjectIsNull becomes false on successful initialization
        responseCache.insert(id, &newResponse);
        return newResponse;
    } else {
        delete newResponse;  // Clean up if initialization failed
        return nullptr;
    }
}

Response::Response(Key matterid, Date triggerDate) : Nutmeg::Task()
{
    Key newid = Nutdb::InsertResponse(matterid, triggerDate);
    if(newid == 0)
    {
        mObjectIsNull = true;
        return;
    }
    else
    {
        InitializeResponse(newid);
    }
}

bool Response::slotUpdate(ResponseData dat)
{
    bool result = Nutdb::UpdateResponse(dat);
    if(!result) return false;

    result = Task::Commit();
    if(!result) return false;

    return InitializeResponse(dat.ResponseId);
}

bool Response::SetId(Key id)
{
    return InitializeResponse(id);
}

bool Response::Commit()
{
    return slotUpdate(mDat);
}

bool Response::slotSetfkClientOfficeHours(Key newval)
{
    bool result = WriteKey(responseTableName, "fkClientOfficeHours", newval);
    if(result)
        mDat.fkClientOfficeHours = newval;
    return result;
}

bool Response::slotSetfkExaminerInterview(Key newval)
{
    bool result = WriteKey(responseTableName, "fkExaminerInterview", newval);
    if(result)
        mDat.fkExaminerInterview = newval;
    return result;
}

bool Response::slotSetMailingDate(Date newval)
{
    bool result = WriteDate(responseTableName, "MailingDate", newval);
    if(result)
        mDat.MailingDate = newval;
    return result;
}

bool Response::slotSetDateFiled(Date newval)
{
    bool result = WriteDate(responseTableName, "DateFiled", newval);
    if(result)
        mDat.DateFiled = newval;
    return result;
}

bool Response::slotSetfkResponseAsFiled(Key newval)
{
    bool result = WriteKey(responseTableName, "fkResponseAsFiled", newval);
    if(result)
        mDat.fkResponseAsFiled = newval;
    return result;
}

bool Response::slotSetfkActionDocument(Key newval)
{
    bool result = WriteKey(responseTableName, "fkActionDocument", newval);
    if(result)
        mDat.fkActionDocument = newval;
    return result;
}

bool Response::InitializeResponse(Key newid)
{
    if(newid == 0)
    {
        mDat = ResponseData();
        return InitializeTask(0);
    }

    mDat = Nutdb::GetResponse(newid);
    if(mDat.ResponseId == 0) return InitializeResponse(0);

    FindNeedsExaminerInterview();
    return InitializeTask(newid);
}

} // namespace Nutmeg
