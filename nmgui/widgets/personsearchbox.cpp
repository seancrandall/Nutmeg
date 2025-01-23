#include "personsearchbox.h"

namespace Nutmeg
{

PersonSearchBox::PersonSearchBox(QWidget *parent) : Nutmeg::AbstractSearchBox(parent)
{
    Initialize();
    key = 0;
}

PersonSearchBox::PersonSearchBox(Key initkey, QWidget *parent) : Nutmeg::AbstractSearchBox(parent)
{
    Initialize();
    key = initkey;
    // emit signalKeySelected(key);
}

void PersonSearchBox::slotAddRecord(const QString &input)
{
    Q_UNUSED(input);

    if (isNew)
    {
        InsertPersonDialog *diag = new InsertPersonDialog(FirstName, LastName, this);
        key = diag->exec();
    }
}

void PersonSearchBox::Initialize()
{
    setTableModel(new viewPeopleModel(this));
    column = 3;

    setMinimumWidth(120);

    completer = new PeopleCompleter(this);
    setCompleter(completer);
}

} // namespace Nutmeg
