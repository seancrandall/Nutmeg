#include "personsearchbox.h"
#include "dbaccess/models.h"

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

QString PersonSearchBox::getFirstName()
{
    mFirstName = Nutmeg::inferName(currentText())[0];
    return mFirstName;
}

QString PersonSearchBox::getLastName()
{
    mLastName = Nutmeg::inferName(currentText())[1];
    return mLastName;
}

void PersonSearchBox::slotAddRecord(const QString &input)
{
     Q_UNUSED(input);

    // if (isNew)
    // {
    //     InsertPersonDialog *diag = new InsertPersonDialog(FirstName, LastName, this);
    //     key = diag->exec();
    // }
}

void PersonSearchBox::Initialize()
{
    if(!gViewPeopleModel)
        gViewPeopleModel = std::make_unique<viewPeopleModel>();
    mModel = gViewPeopleModel.get();
    setTableModel(mModel);
    column = 3;

    setMinimumWidth(120);

    completer = new PeopleCompleter(this);
    setCompleter(completer);
}

} // namespace Nutmeg
