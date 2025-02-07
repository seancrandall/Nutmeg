
#include "entitypanel.h"

namespace Nutmeg {


EntityPanel::EntityPanel(QWidget *parent)
    : Frame(parent)
{
    mEntity = std::make_shared<Entity>(0);
    Initialize();
}

EntityPanel::EntityPanel(Key entityId, QWidget *parent)
    : Frame(parent)
{
    mEntity = std::make_shared<Entity>(entityId);
    Initialize();
}

EntityPanel::EntityPanel(std::shared_ptr<Entity> entity, QWidget *parent)
    : Frame(parent)
{
    mEntity = entity;
    Initialize();
}

void EntityPanel::scatter()
{
    cName->text = mEntity->EntityName;
    cFullLegalName->text = mEntity->FullLegalName;
    cWebsite->text = mEntity->Website;
    cPrimaryPhone->text = mEntity->PrimaryPhone;
    cSecondaryPhone->text = mEntity->SecondaryPhone;
    cFaxNumber->text = mEntity->FaxNumber;
    cPrimaryEmail->text = mEntity->PrimaryEmail;
    cSecondaryEmail->text = mEntity->SecondaryEmail;
    cDocketPrefix->text = mEntity->DocketPrefix;
    cPrimaryAddress->text = mEntity->PrimaryAddress;
    cSecondaryAddress->text = mEntity->SecondaryAddress;
}

void EntityPanel::gather()
{
    mEntity->holdEntityName(cName->text);
    mEntity->holdFullLegalName(cFullLegalName->text);
    mEntity->holdWebsite(cWebsite->text);
    mEntity->holdPrimaryPhone(cPrimaryPhone->text);
    mEntity->holdSecondaryPhone(cSecondaryPhone->text);
    mEntity->holdFaxNumber(cFaxNumber->text);
    mEntity->holdPrimaryEmail(cPrimaryEmail->text);
    mEntity->holdSecondaryEmail(cSecondaryEmail->text);
    mEntity->holdDocketPrefix(cDocketPrefix->text);
    mEntity->holdPrimaryAddress(cPrimaryAddress->text);
    mEntity->holdSecondaryAddress(cSecondaryAddress->text);
    mEntity->Commit();
}

void EntityPanel::Initialize(void)
{
    InitializeObjects();
    LayoutControls();
    scatter();
}

void EntityPanel::InitializeObjects()
{
    cId = new LineDisplayId(mEntity->EntityId);
    cFullLegalName = new LineEdit();
    cWebsite = new LineEdit();
    cPrimaryPhone = new LineEdit();
    cSecondaryPhone = new LineEdit();
    cFaxNumber = new LineEdit();
    cPrimaryEmail = new LineEdit();
    cSecondaryEmail = new LineEdit();
    cDocketPrefix = new LineEdit();
    cPrimaryAddress = new TextEdit();
    cSecondaryAddress = new TextEdit();
    //cState = new ComboBox();
    //cJurisdiction = new ComboBox();

    lId = new LabeledWidgetLeft("ID", cId);
    lName = new LabeledWidgetLeft("Name", cName);
    lFullLegalName = new LabeledWidgetLeft("Full Legal Name", cFullLegalName);
    lPrimaryAddress = new LabeledWidgetLeft("Primary Address", cPrimaryAddress);
    lSecondaryAddress = new LabeledWidgetLeft("Secondary Address", cSecondaryAddress);
    lWebsite = new LabeledWidgetLeft("Website", cWebsite);
    lPrimaryPhone = new LabeledWidgetLeft("Primary Phone", cPrimaryPhone);
    lSecondaryPhone = new LabeledWidgetLeft("Secondary Phone", cSecondaryPhone);
    lPrimaryEmail = new LabeledWidgetLeft("Primary Email", cPrimaryEmail);
    lSecondaryEmail = new LabeledWidgetLeft("Secondary Email", cSecondaryEmail);
    lDocketPrefix = new LabeledWidgetLeft("Docket Prefix", cDocketPrefix);
}

void EntityPanel::LayoutControls()
{
    QGridLayout *grid = new QGridLayout(this);

    grid->addLayout(lId, 0, 0);
    grid->addLayout(lName, 1, 0);
    grid->addLayout(lFullLegalName, 1, 1);
    grid->addLayout(lDocketPrefix, 2, 0);
    grid->addLayout(lWebsite, 2, 1);
    grid->addLayout(lPrimaryPhone, 3, 0);
    grid->addLayout(lSecondaryPhone, 3, 1);
    grid->addLayout(lPrimaryEmail, 4, 0);
    grid->addLayout(lSecondaryEmail, 4, 1);
    grid->addLayout(lPrimaryAddress, 5, 0);
    grid->addLayout(lSecondaryAddress, 5, 0);
}

} // namespace Nutmeg
