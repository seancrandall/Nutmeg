#include "dialog.h"

namespace Nutmeg
{

Dialog::Dialog(QWidget *parent) : QDialog(parent)
{
    setModal(true);
    setSizeGripEnabled(true);
}

Dialog::~Dialog() {}

void Dialog::slotHandleRejected()
{
    emit rejected();
    close();
}

void Dialog::slotHandleAccepted()
{
    slotGather();
    emit accepted();
    close();
}

void Dialog::connectSignalsAndSlots()
{
    QObject::connect(mButtonBox, &QDialogButtonBox::accepted, this, &Nutmeg::Dialog::slotHandleAccepted,
                     Qt::UniqueConnection);

    QObject::connect(mButtonBox, &QDialogButtonBox::rejected, this, &Nutmeg::Dialog::slotHandleRejected,
                     Qt::UniqueConnection);
}

void Dialog::updateLayout()
{
    workspace->setLayout(workLayout);
    mainLayout->addWidget(workspace);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    adjustSize();
}

} // namespace Nutmeg
