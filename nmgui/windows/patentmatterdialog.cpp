#include "patentmatterdialog.h"
#include "qscrollarea.h"
#include "widgets/inventorbutton.h"
#include "windows/insertentitydialog.h"
#include "windows/insertinventordialog.h"
#include "windows/insertpatentexaminerdialog.h"

namespace Nutmeg
{

PatentMatterDialog::PatentMatterDialog(Key pmkey, QWidget *parent)
    : MatterDialog(pmkey, parent), pmatter(PatentMatter(pmkey))
{

    LayoutWidgets();
    connectSignalsAndSlots();
    slotScatter();
}

void PatentMatterDialog::slotScatter()
{
    // Initialize Widgets
    cCriticalDate->date = pmatter.CriticalDate;
    cBarDate->date = pmatter.BarDate;
    cFilingDate->date = pmatter.FilingDate;
    cApplicationSerialNumber->text = pmatter.ApplicationSerialNumber;

    cConfirmationNumber->text = pmatter.ConfirmationNumber;
    cArtUnit->text = pmatter.ArtUnit;
    cPatentNumber->text = pmatter.PatentNumber;
    cExaminer->key = pmatter.fkExaminer;
    cSupervisoryExaminer->key = pmatter.fkSupervisoryExaminer;
    cFirstInventor->key = pmatter.fkFirstInventor;
    cApplicant->key = pmatter.fkApplicant;

    MatterDialog::slotScatter();
}

void PatentMatterDialog::slotGather()
{
    pmatter.CriticalDate = cCriticalDate->date;
    pmatter.BarDate = cBarDate->date;
    pmatter.FilingDate = cFilingDate->date;
    pmatter.ApplicationSerialNumber = cApplicationSerialNumber->text;
    pmatter.ConfirmationNumber = cConfirmationNumber->text;
    pmatter.ArtUnit = cArtUnit->text;
    pmatter.PatentNumber = cPatentNumber->text;
    pmatter.fkExaminer = cExaminer->key;
    pmatter.fkSupervisoryExaminer = cSupervisoryExaminer->key;
    pmatter.fkFirstInventor = cFirstInventor->key;
    //Key visibleKey = cApplicant->key;
    pmatter.fkApplicant = cApplicant->key;

    MatterDialog::slotGather();
}

void PatentMatterDialog::connectSignalsAndSlots()
{
    MatterDialog::connectSignalsAndSlots();

    QObject::connect(cExaminer, &ExaminerEditAdd::signalAddNew, this, &PatentMatterDialog::slotInsertPatentExaminer,
                     Qt::UniqueConnection);

    QObject::connect(cFirstInventor, &InventorEditAdd::signalAddNew, this, &PatentMatterDialog::slotInsertFirstInventor,
                     Qt::UniqueConnection);

    QObject::connect(cSupervisoryExaminer, &ExaminerEditAdd::signalAddNew, this,
                     &PatentMatterDialog::slotInsertSupervisoryExaminer, Qt::UniqueConnection);

    QObject::connect(cpbInsertApplicant, &QPushButton::clicked, this, &PatentMatterDialog::slotInsertApplicant);
}

void PatentMatterDialog::slotInsertPatentExaminer()
{
    InsertPatentExaminerDialog diag(this);
    Key kExaminer = diag.exec();
    cExaminer->key = kExaminer;
    cExaminer->refresh();
}

void PatentMatterDialog::slotInsertSupervisoryExaminer() {}

void PatentMatterDialog::slotInsertFirstInventor()
{
    InsertInventorDialog inv = InsertInventorDialog(this);
    Key kInv = inv.exec();
    cFirstInventor->key = kInv;
    cFirstInventor->refresh();
}

void PatentMatterDialog::slotInsertApplicant()
{
    InsertEntityDialog app = InsertEntityDialog(this);
    Key kApp = app.exec();
    cApplicant->refresh();
    cApplicant->key = kApp;
}

void PatentMatterDialog::LayoutWidgets()
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

    QGridLayout *pmgrid = new QGridLayout();
    // Layout the rest of the group box
    pmgrid->addLayout(lApplicationSearialNumber, 0, 0);
    // lFirstInventor->addWidget(cpbInsertFirstInventor);
    // pmgrid->addLayout(lFirstInventor, 1, 0);
    pmgrid->addWidget(cFirstInventor);
    lApplicant->addWidget(cpbInsertApplicant);
    pmgrid->addLayout(lApplicant, 1, 1);
    pmgrid->addLayout(lConfirmationNumber, 2, 0);
    pmgrid->addLayout(lArtUnit, 2, 1);
    // pmgrid->addLayout(lExaminer, 3, 0);
    // pmgrid->addLayout(lSupervisoryExaminer, 3, 1);
    pmgrid->addWidget(cExaminer, 3, 0);
    pmgrid->addWidget(cSupervisoryExaminer, 3, 1);
    pmgrid->addWidget(dateGroup, 4, 0);
    pmgrid->addWidget(inventorBox, 4, 1);

    gb->setLayout(pmgrid);
    workLayout->addWidget(gb, 1, 0);
    updateLayout();
}

} // namespace Nutmeg
