#include "insertdialog.h"

namespace Nutmeg
{

InsertDialog::InsertDialog(QWidget *parent) : Nutmeg::Dialog(parent) {}

void InsertDialog::slotHandleAccepted()
{
    emit signalKeyCreated(mNewkey);
    done(mNewkey);
    // Dialog::slotHandleAccepted();
}

void InsertDialog::slotHandleRejected()
{
    Dialog::slotHandleRejected();
}

void InsertDialog::connectSignalsAndSlots() { Dialog::connectSignalsAndSlots(); }

} // namespace Nutmeg
