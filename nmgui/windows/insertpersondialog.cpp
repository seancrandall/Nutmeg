#include "insertpersondialog.h"
#include "dbaccess/nutdb.h"

namespace Nutmeg
{

InsertPersonDialog::InsertPersonDialog(QWidget *parent) : InsertDialog(parent)
{
    ppanel = new PersonAddSelectPanel(this);
    this->setupDisplay();
    connectSignalsAndSlots();
    ppanel->setFocus();
}

InsertPersonDialog::InsertPersonDialog(QString firstName, QString LastName, QWidget *parent) : InsertDialog(parent)
{
    ppanel = new PersonAddSelectPanel(this);
    ppanel->FirstName = firstName;
    ppanel->LastName = LastName;
    this->setupDisplay();
    connectSignalsAndSlots();
    ppanel->setFocus();
}

void InsertPersonDialog::slotHandleAccepted()
{
    //mNewkey = Nutdb::InsertPerson(ppanel->FirstName, ppanel->LastName);
    //setResult(mNewkey);
    mNewkey = ppanel->accepted();
    setResult(mNewkey);
    InsertDialog::slotHandleAccepted();
}

void InsertPersonDialog::slotHandleRejected()
{
    setResult(0);
    InsertDialog::slotHandleRejected();
}

void InsertPersonDialog::connectSignalsAndSlots() { InsertDialog::connectSignalsAndSlots(); }

void InsertPersonDialog::setupDisplay()
{
    setWindowTitle("Insert a New Person");

    workLayout->addWidget(ppanel);
    updateLayout();
}

} // namespace Nutmeg
