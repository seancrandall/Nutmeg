#include "matterpanel.h"

namespace Nutmeg {

MatterPanel::MatterPanel(Matter* matterin, QWidget *parent)
    : GroupBox("Matter Data", parent)
    , matter(matterin)
{
    initializeObjects();
    setupLayouts();
    slotScatter();
    ConnectSignalsAndSlots();
}

MatterPanel::MatterPanel(Key matterid, QWidget *parent)
    : GroupBox("Matter Data", parent)
    , matter(new Matter(matterid, this))
{
    initializeObjects();
    setupLayouts();
    slotScatter();
    ConnectSignalsAndSlots();
}

void MatterPanel::ConnectSignalsAndSlots()
{

}

void MatterPanel::slotScatter()
{
    cDocketNumber->text = matter->AttorneyDocketNumber;
    cParent->key = matter->fkParent;
    cClientDocket->text = matter->ClientDocketNumber;
    cClient->key = matter->fkClient;
    cAssigningFirm->key = matter->fkAssigningFirm;
    cDefaultWorkAttorney->key = matter->fkDefaultWorkAttorney;
    cDefaultParalegal->key = matter->fkDefaultParalegal;
    cJurisdiction->key = matter->fkMatterJurisdiction;
}

void MatterPanel::slotGather()
{
    // matter.id is not editable
#ifdef QT_DEBUG
    Key parentKey = cParent->key;
    qDebug() << "Parent key is: " << parentKey;
#endif
    matter->fkParent = cParent->key;
    matter->AttorneyDocketNumber = cDocketNumber->text;
    matter->ClientDocketNumber = cClientDocket->text;
    matter->Title = cTitle->text;
    matter->fkClient = cClient->key;
    matter->fkAssigningFirm = cAssigningFirm->key;
    matter->fkDefaultWorkAttorney = cDefaultWorkAttorney->key;
    matter->fkDefaultParalegal = cDefaultParalegal->key;
    matter->fkMatterJurisdiction = cJurisdiction->key;
}

void MatterPanel::initializeObjects()
{
    grid = new QGridLayout();
    fullLayout = new QGridLayout();

    cId = new LineDisplayId(matter->MatterId);

    cDocketNumber = new LineEdit();

    cParent = new MatterSearchBox();

    cClientDocket = new LineEdit();

    cClient = new ComboBox();
    cClient->model = new viewClientsModel();
    cClient->column = 3;

    cTitle = new TitleEdit(matter);

    cAssigningFirm = new ComboBox();
    cAssigningFirm->model = new viewContractingFirmsModel(this);
    cAssigningFirm->column = 1;

    cDefaultWorkAttorney = new ComboBox();
    cDefaultWorkAttorney->model = new viewWorkAttorneysModel();
    cDefaultWorkAttorney->column = 3;

    cDefaultParalegal = new ComboBox();
    cDefaultParalegal->model = new viewParalegalsModel();
    cDefaultParalegal->column = 9;

    cJurisdiction = new ComboBox();
    cJurisdiction->model = new viewJurisdictionsModel();
    cJurisdiction->column = 1;
}

void MatterPanel::setupLayouts()
{
    lId = new LabeledWidgetLeft("Matter Id", cId);
    lDocketNumber = new LabeledWidgetLeft("Attorney Docket Number", cDocketNumber);
    lParent = new LabeledWidgetLeft("Parent Matter", cParent);
    lClient = new LabeledWidgetLeft("Client", cClient);
    lClientDocket = new LabeledWidgetLeft("Client Docket Number", cClientDocket);
    lTitle = new LabeledWidgetLeft("Title", cTitle);
    lDefaultWorkAttorney = new LabeledWidgetLeft("Default Work Attorney", cDefaultWorkAttorney);
    lDefaultParalegal = new LabeledWidgetLeft("Default Paralegal", cDefaultParalegal);
    lJurisdiction = new LabeledWidgetLeft("Matter Jurisdiction", cJurisdiction);

    grid->addLayout(lId, 0, 0);
    grid->addLayout(lDocketNumber, 1, 0);
    grid->addLayout(lParent, 1, 1);
    grid->addLayout(lClient, 2, 0);
    grid->addLayout(lClientDocket, 2, 1);
    grid->addLayout(lTitle, 3, 0);
    grid->addLayout(lDefaultWorkAttorney, 4, 0);
    grid->addLayout(lDefaultParalegal, 4, 1);
    grid->addLayout(lJurisdiction, 5, 0);

    setLayout(grid);
}

} // namespace Nutmeg
