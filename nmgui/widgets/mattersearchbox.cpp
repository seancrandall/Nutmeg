#include "mattersearchbox.h"
#include "dbaccess/models.h"

namespace Nutmeg
{

MatterSearchBox::MatterSearchBox(QWidget *parent) : AbstractSearchBox(parent)
{
    Initialize();
    key = 0;
}

MatterSearchBox::MatterSearchBox(Key initkey, QWidget *parent) : AbstractSearchBox(parent)
{
    Initialize();
    key = initkey;
    emit signalKeySelected(initkey);
}

void MatterSearchBox::slotAddRecord(const QString &input)
{
    // Key selectedKey;

    // isNew is a property that does a search, so no need to replicate it
    bool found = isNew;

    if (found)
    {
        // Nothing to do. Record already exists
        return;
    }
    else
    {
        // If the input does not match any record, insert a new matter
        Key newKey = Nutdb::InsertMatter(input);
        // Refresh the combo box model (usually you would re-query the data)
        mModel->select();
        // Set the combo box to the new record
        // setToKey(newKey);
        key = newKey;
    }

    emit signalKeySelected(key);
}

void MatterSearchBox::Initialize()
{
    if(!gViewMattersModel)
        gViewMattersModel = std::make_unique<viewMattersModel>();
    mModel = gViewMattersModel.get();
    setTableModel(mModel);
    column = 4;

    setMinimumWidth(120);

    completer = new MatterCompleter(this);
    setCompleter(completer);
}

} // namespace Nutmeg
