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

bool Document::slotSetId(Key newval) { TRY(InitializeDocument(newval);) }

bool Document::slotUpdate(DocumentData dat) { TRY(Nutdb::UpdateDocument(dat); InitializeDocument(dat.DocumentId);) }

bool Document::slotCommit() { TRY(slotUpdate(mDat); Object::slotCommit();) }

void Document::slotSetFullyQualifiedPath(String newval)
{
    TRY(mDat.FullyQualifiedPath = newval;

        // We can derive the other properties here.

        UpdateMacro;)
}

void Document::slotSetURL(String newval)
{
    TRY(mDat.URL = newval;

        // We can derive the other properties here.

        UpdateMacro;)
}

void Document::InitializeDocument(Key id) { TRY(mDat = Nutdb::GetDocument(id); Object::InitializeObject(id);) }

} // namespace Nutmeg
