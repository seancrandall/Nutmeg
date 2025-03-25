#include "newresponsedialog.h"
// #include "dbaccess/nutdb.h"
#include "dbaccess/viewcopyrighttasktypesmodel.h"
#include "dbaccess/viewpatenttasktypesmodel.h"
#include "dbaccess/viewtaskclassmodel.h"
#include "dbaccess/viewtasktypesmodel.h"
#include "dbaccess/viewtrademarktasktypesmodel.h"
#include "nutmeg.h"
#include "objects/deadline.h"
#include "objects/response.h"

NewResponseDialog::NewResponseDialog(QWidget *parent) : QDialog(parent), ui(new Ui::NewResponseDialog)
{
    ui->setupUi(this);
    ui->comboMatter->instantInsert = false;
    setWindowTitle("Add New Response Task");

    SetupCombos();
    ConnectSignals();
}

NewResponseDialog::~NewResponseDialog() { delete ui; }

void NewResponseDialog::slotCreateMatter()
{
    Nutmeg::Key classSelection = ui->comboTaskClass->key;
    Nutmeg::Key newMatterKey = 0;
    QString docket = ui->comboMatter->AttorneyDocketNumber;

  TRY(switch (classSelection) {
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
	  ui->comboMatter = new Nutmeg::MatterSearchBox(this); ui->comboMatter->key = newMatterKey;)
}

void NewResponseDialog::slotCreateTask()
{
    Nutmeg::Key matterKey = ui->comboMatter->key;
    Nutmeg::Key responseKey = 0;
    Nutmeg::Key taskClass = ui->comboTaskClass->key;
    Nutmeg::Key taskType = ui->comboTask->key;

    switch (taskClass)
    {
    case 1:
        responseKey = Nutmeg::Nutdb::InsertPatentResponse(matterKey);
        break;
    case 2:
        responseKey = Nutmeg::Nutdb::InsertTrademarkResponse(matterKey);
        break;
    case 3:
    case 4:
    case 7:
        responseKey = Nutmeg::Nutdb::InsertResponse(matterKey);
        break;
    }

    Nutmeg::Response resp = Nutmeg::Response(responseKey);
    resp.fkTaskType = taskType;
    Nutmeg::Key fkDeadline = resp.fkDeadline;
    Nutmeg::Deadline deadline = Nutmeg::Deadline(fkDeadline);

    deadline.TriggerDate = ui->deTriggerDate->date;
    deadline.SoftDeadline = ui->deSoftDeadline->date;
    deadline.HardDeadline = ui->deHardDeadline->date;
    deadline.NextDeadline = ui->deNextDeadline->date;

    return;
}

void NewResponseDialog::slotTaskRejected()
{
    emit signalDialogDone();
    close();
}

/// See which task class we have and load the correct task list
void NewResponseDialog::slotUpdateTaskList()
{
    Nutmeg::Key classSelection = ui->comboTaskClass->key;

    switch (classSelection)
    {
    case 1:
        tasktypemodel = new Nutmeg::viewPatentTaskTypesModel(this);
        ui->comboTask->model = tasktypemodel;
        break;
    case 2:
        tasktypemodel = new Nutmeg::viewTrademarkTaskTypesModel(this);
        ui->comboTask->model = tasktypemodel;
        break;
    case 3:
        tasktypemodel = new Nutmeg::viewCopyrightTaskTypesModel(this);
        ui->comboTask->model = tasktypemodel;
        break;
    case 4:
    case 7:
        tasktypemodel = new Nutmeg::viewTaskTypesModel(this);
        ui->comboTask->model = tasktypemodel;
        break;
    }
}

void NewResponseDialog::on_NewResponseDialog_accepted()
{
    if (ui->comboMatter->isNew)
        slotCreateMatter();
    slotCreateTask();

    emit signalDialogDone();
    close();
}

void NewResponseDialog::on_NewResponseDialog_rejected() { slotTaskRejected(); }

void NewResponseDialog::SetupCombos()
{
    Nutmeg::viewTaskClassModel *tcmodel = new Nutmeg::viewTaskClassModel(this);
    ui->comboTaskClass->model = tcmodel;
    ui->comboTaskClass->column = 1;
    ui->comboTaskClass->key = 1;

    tasktypemodel = new Nutmeg::viewPatentTaskTypesModel(this);
    ui->comboTask->model = tasktypemodel;
    ui->comboTask->column = 2;
}

void NewResponseDialog::ConnectSignals()
{
    connect(ui->comboTaskClass, &QComboBox::currentIndexChanged, this, &NewResponseDialog::slotUpdateTaskList);
}
