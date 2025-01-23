#include "insertpersondialog.h"
#include "dbaccess/nutdb.h"

namespace Nutmeg
{

InsertPersonDialog::InsertPersonDialog(QWidget *parent) : InsertDialog(parent)
{
    this->setupDisplay();
    connectSignalsAndSlots();
}

InsertPersonDialog::InsertPersonDialog(QString firstName, QString LastName, QWidget *parent) : InsertDialog(parent)
{
    ppanel->FirstName = firstName;
    ppanel->LastName = LastName;
    this->setupDisplay();
    connectSignalsAndSlots();
}

void InsertPersonDialog::slotHandleAccepted()
{
    mNewkey = Nutdb::InsertPerson(ppanel->FirstName, ppanel->LastName);
    InsertDialog::slotHandleAccepted();
}

void InsertPersonDialog::slotHandleRejected() { InsertDialog::slotHandleRejected(); }

void InsertPersonDialog::connectSignalsAndSlots() { InsertDialog::connectSignalsAndSlots(); }

void InsertPersonDialog::setupDisplay()
{
    setWindowTitle("Insert a New Person");

    workLayout->addWidget(ppanel);
    updateLayout();
}

} // namespace Nutmeg
