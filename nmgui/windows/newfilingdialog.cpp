#include "newfilingdialog.h"

namespace Nutmeg
{

NewFilingDialog::NewFilingDialog(QWidget *parent) : QDialog(parent), ui(new Ui::NewFilingDialog)
{
    ui->setupUi(this);
    ui->comboMatter->instantInsert = false;
    setWindowTitle("Add New Filing Task");

    SetupCombos();
    ConnectSignals();
}

NewFilingDialog::~NewFilingDialog() { delete ui; }

void NewFilingDialog::slotCreateMatter()
{
    Nutmeg::Key classSelection = ui->comboTaskClass->key;
    Nutmeg::Key newMatterKey = 0;
    QString docket = ui->comboMatter->AttorneyDocketNumber;

  switch (classSelection) {
        case 1:
            newMatterKey = Nutmeg::Nutdb::InsertPatentMatter(docket);
            break;
        case 2:
            newMatterKey = Nutmeg::Nutdb::InsertTrademarkMatter(docket);
            break;
        case 3:
            newMatterKey = Nutmeg::Nutdb::InsertCopyrightMatter(docket);
            break;
        case 4:
        case 7:
            newMatterKey = Nutmeg::Nutdb::InsertMatter(docket);
            break;
  }

	  delete ui->comboMatter;
      ui->comboMatter = new Nutmeg::MatterSearchBox(this); ui->comboMatter->key = newMatterKey;
}

void NewFilingDialog::slotCreateTask()
{
    Nutmeg::Key matterKey = ui->comboMatter->key;
    Nutmeg::Key filingKey = 0;
    Nutmeg::Key taskClass = ui->comboTaskClass->key;

    switch (taskClass)
    {
    case 1:
        filingKey = Nutmeg::Nutdb::InsertPatentFiling(matterKey);
        break;
    case 2:
        filingKey = Nutmeg::Nutdb::InsertTrademarkFiling(matterKey);
        break;
    case 3:
    case 4:
    case 7:
        filingKey = Nutmeg::Nutdb::InsertFiling(matterKey);
        break;
    }

    Nutmeg::Filing filing(filingKey);
    Nutmeg::Key fkDeadline = filing.fkDeadline;
    Nutmeg::Deadline deadline(fkDeadline);

    deadline.TriggerDate = ui->deTriggerDate->date;
    deadline.SoftDeadline = ui->deSoftDeadline->date;
    deadline.HardDeadline = ui->deHardDeadline->date;
    deadline.NextDeadline = ui->deNextDeadline->date;

    return;
}

void NewFilingDialog::slotTaskRejected()
{
    emit signalDialogDone();
    close();
}

void NewFilingDialog::slotUpdateTaskList()
{
    Nutmeg::Key classSelection = ui->comboTaskClass->key;

    switch (classSelection)
    {
    case 1:
        tasktypemodel = new Nutmeg::viewPatentFilingTypesModel(this);
        ui->comboTask->model = tasktypemodel;
        break;
    case 2:
        tasktypemodel = new Nutmeg::viewTrademarkFilingTypesModel(this);
        ui->comboTask->model = tasktypemodel;
        break;
    case 3:
        tasktypemodel = new Nutmeg::viewCopyrightFilingTypesModel(this);
        ui->comboTask->model = tasktypemodel;
        break;
    case 4:
    case 7:
        tasktypemodel = new Nutmeg::viewFilingTypesModel(this);
        ui->comboTask->model = tasktypemodel;
        break;
    }
}

void NewFilingDialog::slotDialogAccepted()
{
    if (ui->comboMatter->isNew)
        slotCreateMatter();
    slotCreateTask();

    emit signalDialogDone();
    close();
}

void NewFilingDialog::slotDialogRejected()
{
    emit signalDialogDone();
    close();
}

void NewFilingDialog::SetupCombos()
{
    Nutmeg::viewTaskClassModel *tcmodel = new Nutmeg::viewTaskClassModel(this);
    ui->comboTaskClass->model = tcmodel;
    ui->comboTaskClass->column = 1;
    ui->comboTaskClass->key = 1;

    tasktypemodel = new Nutmeg::viewPatentFilingTypesModel(this);
    ui->comboTask->model = tasktypemodel;
    ui->comboTask->column = 2;
}

void NewFilingDialog::ConnectSignals()
{
    connect(ui->comboTaskClass, &QComboBox::currentIndexChanged, this, &NewFilingDialog::slotUpdateTaskList);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &NewFilingDialog::slotDialogAccepted);

    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &NewFilingDialog::slotDialogRejected);
}

} // namespace Nutmeg
