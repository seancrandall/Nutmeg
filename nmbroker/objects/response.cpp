#include "response.h"

namespace Nutmeg
{

Response::Response() : Nutmeg::Task{}
{
    mObjectIsNull = true;
}

Response::Response(Key newid) : Nutmeg::Task(newid)
{
    Response* cachedResponse = cache<Response>::getObjectFromCache(newid, &Response::GetResponse);
    if (cachedResponse) {
        // If we find the Response in cache, copy its state
        *this = *cachedResponse;
        mObjectIsNull = false; // Assuming this is inherited from Object via Task
    } else {
        // If not in cache, proceed with initialization
        InitializeResponse(newid);
    }
}

// Static method to fetch a Response from the database if not in cache
Response* Response::GetResponse(Key id) {
    Response* response = new Response(id); // This will call the constructor again, but now with cache check
    if (response->mObjectIsNull) { // Assuming this is inherited from Object via Task
        delete response; // Clean up if initialization failed
        return nullptr;
    }
    return response;
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
