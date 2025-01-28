#include "document.h"

namespace Nutmeg
{

Document::Document() : Object() {}

Document::Document(Key id)
    : Object(id)
{
    Document* cachedDocument = cache<Document>::getObjectFromCache(id, &Document::GetDocument);
    if (cachedDocument) {
        // If we find the Document in cache, copy its state
        *this = *cachedDocument;
        mObjectIsNull = false; // Assuming this is inherited from Object
    } else {
        // If not in cache, proceed with initialization
        InitializeDocument(id);
    }
}

// Static method to fetch a Document from the database if not in cache
Document* Document::GetDocument(Key id) {
    Document* document = new Document(id); // This will call the constructor again, but now with cache check
    if (document->mObjectIsNull) { // Assuming this is inherited from Object
        delete document; // Clean up if initialization failed
        return nullptr;
    }
    return document;
}
Document::Document(String Title) : Object()
{
    Key newid = Nutdb::InsertDocument(Title);
    InitializeDocument(newid);
}

bool Document::SetId(Key newval)
{
    return InitializeDocument(newval);
}

bool Document::slotUpdate(DocumentData dat)
{
    bool result = Nutdb::UpdateDocument(dat);
    if(!result) return false;

    result = Object::Commit();
    if(!result) return false;

    return InitializeDocument(dat.DocumentId);
}

bool Document::Commit()
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
