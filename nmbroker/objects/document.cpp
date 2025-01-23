#include "document.h"

namespace Nutmeg
{

Document::Document(QObject *parent) : Nutmeg::Object(parent) {}

Document::Document(Key id, QObject *parent) : Object(id, parent) // Call the Object constructor

{
    InitializeDocument(id);
}

Document::Document(String Title, QObject *parent) : Object(parent) // Call the base class constructor
{
    Key newid = Nutdb::InsertDocument(Title);
    InitializeDocument(newid);
}

bool Document::slotSetId(Key newval)
{
    return InitializeDocument(newval);
}

bool Document::slotUpdate(DocumentData dat)
{
    bool result = Nutdb::UpdateDocument(dat);
    if(!result) return false;

    result = Object::slotCommit();
    if(!result) return false;

    return InitializeDocument(dat.DocumentId);
}

bool Document::slotCommit()
{
    return slotUpdate(mDat);
}

bool Document::slotSetFullyQualifiedPath(String newval)
{
    bool result = WriteString(documentTableName, "FullyQualifiedPath", newval);
    if(result)
        mDat.FullyQualifiedPath = newval;
    return result;
}

bool Document::slotSetURL(String newval)
{
    bool result = WriteString(documentTableName, "URL", newval);
    if(result)
        mDat.URL = newval;
    return result;
}

bool Document::InitializeDocument(Key id)
{
    if(id == 0)
    {
        mDat = DocumentData();
        return InitializeObject(0);
    }

    mDat = Nutdb::GetDocument(id);
    if(mDat.DocumentId == 0) return InitializeObject(0);

    return InitializeObject(id);
}

} // namespace Nutmeg
