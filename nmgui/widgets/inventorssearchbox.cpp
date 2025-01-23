#include "inventorssearchbox.h"
#include "informationbox.h"

namespace Nutmeg
{

InventorsSearchBox::InventorsSearchBox(QWidget *parent) : Nutmeg::PersonSearchBox(parent)
{
    Initialize();
    key = 0;
}

InventorsSearchBox::InventorsSearchBox(Key initkey, QWidget *parent) : Nutmeg::PersonSearchBox(initkey, parent)
{
    Initialize();
}

void InventorsSearchBox::slotAddRecord(const QString &input)
{
    Q_UNUSED(input);

    if (isNew)
    {
        InsertPersonDialog *diag = new InsertPersonDialog(FirstName, LastName);
        key = diag->exec();
        emit signalKeyChanged(key);
    }
}

void InventorsSearchBox::Initialize()
{
    setTableModel(new viewInventorsModel(this));
    column = 3;

    setMinimumWidth(120);

    delete PersonSearchBox::completer;
    completer = new InventorCompleter(this);
    setCompleter(completer);
}

} // namespace Nutmeg
