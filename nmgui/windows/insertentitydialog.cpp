#include "insertentitydialog.h"

namespace Nutmeg
{

InsertEntityDialog::InsertEntityDialog(QWidget *parent) : InsertDialog(parent)
{
    setupDisplay();
    connectSignalsAndSlots();
}

InsertEntityDialog::InsertEntityDialog(const QString entityName, QWidget *parent) : InsertDialog(parent)
{
    epanel->EnterpriseName = entityName;
    QStringList names = Nutmeg::inferName(entityName);
    ppanel->FirstName = names[0];
    ppanel->LastName = names[1];
    slotChangeType(EntityType::Enterprise);
}

InsertEntityDialog::InsertEntityDialog(const QString firstName, const QString lastName, QWidget *parent)
    : InsertDialog(parent)
{
    ppanel->FirstName = firstName;
    ppanel->LastName = lastName;
    epanel->EnterpriseName = Nutmeg::inferEnterpriseName(firstName, lastName);
    slotChangeType(EntityType::NaturalPerson);
}

void InsertEntityDialog::slotHandleAccepted()
{
    int et = cType->currentIndex();

    if (et == EntityType::NaturalPerson)
    {
        newkey = Nutdb::InsertPerson(ppanel->FirstName, ppanel->LastName);
        done(newkey);
    }
    else if (et == EntityType::Enterprise)
    {
        newkey = Nutdb::InsertEnterprise(epanel->EnterpriseName);
        done(newkey);
    }
    else
    {
        // ERROR! You shouldn't be here!
        done(0);
        return;
    }
}

void InsertEntityDialog::connectSignalsAndSlots()
{
    InsertDialog::connectSignalsAndSlots();

    QObject::connect(cType, &QComboBox::currentIndexChanged, this, &InsertEntityDialog::slotChangeType,
                     Qt::UniqueConnection);
}

void InsertEntityDialog::setupDisplay()
{
    cType->insertItem(0, "Natural Person");
    cType->insertItem(1, "Enterprise");
    cType->setCurrentIndex(0);

    hlayout->addLayout(lType);
    hlayout->addWidget(ppanel);
    epanel->hide();

    container->setLayout(hlayout);
    workLayout->addWidget(container, 0, 0);

    updateLayout();
}

void InsertEntityDialog::slotChangeType(int et)
{
    if (et == EntityType::NaturalPerson)
    {
        hlayout->removeWidget(epanel);
        epanel->hide();
        hlayout->addWidget(ppanel);
        ppanel->show();
    }
    else if (et == EntityType::Enterprise)
    {
        hlayout->removeWidget(ppanel);
        ppanel->hide();
        hlayout->addWidget(epanel);
        epanel->show();
    }
    else
    {
        // Something went wrong. You shouldn't be here.
        return;
    }
}

} // namespace Nutmeg
