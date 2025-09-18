#include "entitiespanel.h"

namespace Nutmeg
{

EntitiesPanel::EntitiesPanel(Key taskid, QWidget *parent)
    : Frame(parent)
      , task(Task(taskid))
      , matter(Matter(task.fkMatter))
{
    SetupInterface();
    ConnectSignalsAndSlots();
}

EntitiesPanel::EntitiesPanel(const responsesDashboardEntry &entry, QWidget *parent)
    : EntitiesPanel(entry.getTaskId(), parent)
{
    clientsBox->setCurrentText(entry.getClientEntityName());
    paralegalsBox->setCurrentText(entry.getParalegalEntityName());
    attorneysBox->setCurrentText(entry.getWorkAttorneyEntityName());
}

void EntitiesPanel::updateClient(Key newkey)
{
    matter.fkClient = newkey;
}

void EntitiesPanel::updateParalegal(Key newkey)
{
    task.fkParalegal = newkey;
}

void EntitiesPanel::updateAttorney(Key newkey)
{
    task.fkWorkAttorney = newkey;
}

void EntitiesPanel::SetupInterface()
{
    //Setup clients combo
    viewClientsModel *cmodel = new viewClientsModel(this);
    QLabel *clientLabel = new QLabel("Client");
    QVBoxLayout *clientLayout = new QVBoxLayout();
    clientsBox = new ComboBox();
    clientsBox->setMaximumWidth(150);
    clientsBox->model = cmodel;
    clientsBox->column = 3;
    clientsBox->key = matter.fkClient;
    clientLayout->addWidget(clientLabel);
    clientLayout->addWidget(clientsBox);
    clientLayout->addStretch();
    clientLayout->setAlignment(Qt::AlignVCenter);

    //Setup paralegal combo
    viewParalegalsModel *pmodel = new viewParalegalsModel(this);
    QLabel *paralegalLabel = new QLabel("Paralegal");
    QVBoxLayout *paralegalLayout = new QVBoxLayout();
    paralegalsBox = new ComboBox();
    paralegalsBox->setMaximumWidth(150);
    paralegalsBox->model = pmodel;
    paralegalsBox->column = 3;
    paralegalsBox->setToKey(task.fkParalegal);
    paralegalLayout->addWidget(paralegalLabel);
    paralegalLayout->addWidget(paralegalsBox);
    paralegalLayout->addStretch();
    paralegalLayout->setAlignment(Qt::AlignVCenter);

    //Setup work attorney combo
    viewWorkAttorneysModel *amodel = new viewWorkAttorneysModel(this);
    QLabel *attorneyLabel = new QLabel("Work Attorney");
    QVBoxLayout *attorneyLayout = new QVBoxLayout();
    attorneysBox = new ComboBox();
    attorneysBox->setMaximumWidth(150);
    attorneysBox->model = amodel;
    attorneysBox->column = 3;
    attorneysBox->setToKey(task.fkWorkAttorney);
    attorneyLayout->addWidget(attorneyLabel);
    attorneyLayout->addWidget(attorneysBox);
    attorneyLayout->addStretch();
    attorneyLayout->setAlignment(Qt::AlignVCenter);

    QGridLayout *grid = new QGridLayout(this);
    grid->addLayout(clientLayout, 0, 0);
    grid->addLayout(paralegalLayout, 1, 0);
    grid->addLayout(attorneyLayout, 1, 1);
}

void EntitiesPanel::ConnectSignalsAndSlots()
{
    QObject::connect(clientsBox,        &Nutmeg::ComboBox::signalKeyChanged,
                     this,              &Nutmeg::EntitiesPanel::updateClient);
    QObject::connect(paralegalsBox,     &Nutmeg::ComboBox::signalKeyChanged,
                     this,              &Nutmeg::EntitiesPanel::updateParalegal);
    QObject::connect(attorneysBox,      &Nutmeg::ComboBox::signalKeyChanged,
                     this,              &Nutmeg::EntitiesPanel::updateAttorney);
}

} // namespace Nutmeg
