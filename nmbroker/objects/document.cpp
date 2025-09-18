#include "document.h"

namespace Nutmeg
{

Document::Document() : Object() {}
Document::Document(Key id)
    : Object(id)
{
    auto& documentCache = getCache<Document>();
    if (documentCache.contains(id)) {  // Check if Document is already in cache
        Document* cachedDocument = *documentCache.object(id);
        if (cachedDocument) {
            *this = *cachedDocument;  // Copy state if found in cache
            mObjectIsNull = false;
            return;  // Exit constructor early if we've copied from cache
        }
    }
    // If not in cache, proceed with initialization
    InitializeDocument(id);
}

Document* Document::GetDocument(Key id) {
    auto& documentCache = getCache<Document>();
    if (documentCache.contains(id)) {
        return *documentCache.object(id);  // Return from cache if available
    }

    // If not in cache, create new Document and initialize it
    Document* newDocument = new Document(id);
    if (!newDocument->mObjectIsNull) { // Assuming mObjectIsNull becomes false on successful initialization
        documentCache.insert(id, &newDocument);
        return newDocument;
    } else {
        delete newDocument;  // Clean up if initialization failed
        return nullptr;
    }
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

void Document::slotSetFilename(String newval)
{
    bool result = WriteString(documentTableName, "Filename", newval);
    if(result)
        mDat.Filename = newval;
}

void Document::slotSetExtension(String newval)
{
    bool result = WriteString(documentTableName, "Extension", newval);
    if(result)
        mDat.Extension = newval;
}

void Document::slotSetTitle(String newval)
{
    bool result = WriteString(documentTableName, "Title", newval);
    if(result)
        mDat.Title = newval;
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
