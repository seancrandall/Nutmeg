#include "patentmatterpanel.h"

namespace Nutmeg {

PatentMatterPanel::PatentMatterPanel(std::shared_ptr<PatentMatter> patMatterIn, QWidget *parent)
    : GroupBox("Patent Matter Data", parent)
    , patMatter(patMatterIn)
{
    LayoutWidgets();
    slotScatter();
    ConnectSignalsAndSlots();
}

PatentMatterPanel::PatentMatterPanel(Key patentMatterId, QWidget *parent)
    : GroupBox("Patent Matter Data", parent)
    , patMatter(std::make_shared<PatentMatter>(patentMatterId))
{
    LayoutWidgets();
    slotScatter();
    ConnectSignalsAndSlots();
}

void PatentMatterPanel::slotScatter()
{
    // Initialize Widgets
    cCriticalDate->date = patMatter->CriticalDate;
    cBarDate->date = patMatter->BarDate;
    cFilingDate->date = patMatter->FilingDate;
    cApplicationSerialNumber->text = patMatter->ApplicationSerialNumber;

    cConfirmationNumber->text = patMatter->ConfirmationNumber;
    cArtUnit->text = patMatter->ArtUnit;
    cPatentNumber->text = patMatter->PatentNumber;
    cExaminer->key = patMatter->fkExaminer;
    cSupervisoryExaminer->key = patMatter->fkSupervisoryExaminer;
    cFirstInventor->key = patMatter->fkFirstInventor;
    cApplicant->key = patMatter->fkApplicant;
}

void PatentMatterPanel::slotGather()
{
    patMatter->CriticalDate = cCriticalDate->date;
    patMatter->BarDate = cBarDate->date;
    patMatter->FilingDate = cFilingDate->date;
    patMatter->ApplicationSerialNumber = cApplicationSerialNumber->text;
    patMatter->ConfirmationNumber = cConfirmationNumber->text;
    patMatter->ArtUnit = cArtUnit->text;
    patMatter->PatentNumber = cPatentNumber->text;
    patMatter->fkExaminer = cExaminer->key;
    patMatter->fkSupervisoryExaminer = cSupervisoryExaminer->key;
    patMatter->fkFirstInventor = cFirstInventor->key;
    //Key visibleKey = cApplicant->key;
    patMatter->fkApplicant = cApplicant->key;
}

void PatentMatterPanel::ConnectSignalsAndSlots()
{
    QObject::connect(cExaminer, &ExaminerEditAdd::signalAddNew, this, &PatentMatterPanel::slotInsertPatentExaminer,
                     Qt::UniqueConnection);

    QObject::connect(cFirstInventor, &InventorEditAdd::signalAddNew, this, &PatentMatterPanel::slotInsertFirstInventor,
                     Qt::UniqueConnection);

    QObject::connect(cSupervisoryExaminer, &ExaminerEditAdd::signalAddNew, this,
                     &PatentMatterPanel::slotInsertSupervisoryExaminer, Qt::UniqueConnection);

    QObject::connect(cpbInsertApplicant, &QPushButton::clicked, this, &PatentMatterPanel::slotInsertApplicant);
}

void PatentMatterPanel::slotInsertPatentExaminer()
{
    InsertPatentExaminerDialog diag(this);
    Key kExaminer = diag.exec();
    cExaminer->key = kExaminer;
    cExaminer->refresh();
}

void PatentMatterPanel::slotInsertSupervisoryExaminer() {}

void PatentMatterPanel::slotInsertFirstInventor()
{
    InsertInventorDialog inv = InsertInventorDialog(this);
    Key kInv = inv.exec();
    cFirstInventor->key = kInv;
    cFirstInventor->refresh();
}

void PatentMatterPanel::slotInsertApplicant()
{
    InsertEntityDialog app = InsertEntityDialog(this);
    Key kApp = app.exec();
    cApplicant->refresh();
    cApplicant->key = kApp;
}

void PatentMatterPanel::LayoutWidgets()
{
    // Layout the date group
    QVBoxLayout *datev = new QVBoxLayout();
    datev->addLayout(lCriticalDate);
    datev->addLayout(lBarDate);
    datev->addLayout(lFilingDate);
    dateGroup->setLayout(datev);

    // Layout the inventors box
    iblayout->addWidget(canInsertInventor);
    // For each inventor, add a link or whatever
    iblayout->addWidget(new InventorButton(167));
    iblayout->addWidget(new InventorButton(135));
    iblayout->addWidget(new InventorButton(92));
    iblayout->addWidget(new InventorButton(84));
    iblayout->addWidget(new InventorButton(68));
    iblayout->addWidget(new InventorButton(124));
    ibFrame->setLayout(iblayout);
    QScrollArea *inventorScroll = new QScrollArea();
    inventorScroll->setWidget(ibFrame);
    QVBoxLayout *scrollLayout = new QVBoxLayout();
    scrollLayout->addWidget(inventorScroll);
    inventorBox->setLayout(scrollLayout);

    QGridLayout *pmgrid = new QGridLayout(this);
    // Layout the rest of the group box
    pmgrid->addLayout(lApplicationSearialNumber, 0, 0);
    pmgrid->addWidget(cFirstInventor);
    lApplicant->addWidget(cpbInsertApplicant);
    pmgrid->addLayout(lApplicant, 1, 1);
    pmgrid->addLayout(lConfirmationNumber, 2, 0);
    pmgrid->addLayout(lArtUnit, 2, 1);
    pmgrid->addWidget(cExaminer, 3, 0);
    pmgrid->addWidget(cSupervisoryExaminer, 3, 1);
    pmgrid->addWidget(dateGroup, 4, 0);
    pmgrid->addWidget(inventorBox, 4, 1);

    setLayout(pmgrid);
    //workLayout->addWidget(gb, 1, 0);
    //updateLayout();
}

} // namespace Nutmeg
