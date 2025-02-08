#include <QGroupBox>
#include <QScrollArea>

#include "inventorspanel.h"
#include "objects/person.h"
#include "windows/entitydialog.h"
#include "windows/insertinventordialog.h"

namespace Nutmeg {

InventorsPanel::InventorsPanel(QWidget *parent)
    : Frame(parent)
{
    mPatentMatter = std::make_shared<PatentMatter>(0);
    Initialize();
}

InventorsPanel::InventorsPanel(Key patentMatterId, QWidget *parent)
    : Frame(parent)
{
    mPatentMatter = std::make_shared<PatentMatter>(patentMatterId);
    Initialize();
}

InventorsPanel::InventorsPanel(std::shared_ptr<PatentMatter> patentMatter, QWidget *parent)
    : Frame(parent)
{
    mPatentMatter = patentMatter;
    Initialize();
}

void InventorsPanel::AddInventor()
{
    InsertInventorDialog *diag = new InsertInventorDialog(this);
    Key newkey = diag->exec();
    if(newkey)
        mPatentMatter->AssignInventor(newkey);
}

void InventorsPanel::RemoveInventor(Key inventorId)
{
    mPatentMatter->RemoveInventor(inventorId);
}

void InventorsPanel::OpenInventor(Key inventorId)
{
    EntityDialog diag(inventorId);
    diag.setModal(true);
    diag.exec();
}

void InventorsPanel::Initialize()
{
    LoadInventors();
    InitializeObjects();
    LayoutWidgets();
    ConnectSignalsAndSlots();
}

void InventorsPanel::LoadInventors()
{
    mInventorList = mPatentMatter->inventors;
}

void InventorsPanel::InitializeObjects()
{
    cAddInventor = new AddNewButton();

    for(auto i=0; i < mInventorList.size(); i++){
        cInventorButtons << new InventorButton(mInventorList[i]);
    }
}

void InventorsPanel::LayoutWidgets()
{
    QGroupBox *groupBox = new QGroupBox("Inventors");
    QVBoxLayout *ibuttonsLayout = new QVBoxLayout();
    //QVBoxLayout *groupBoxLayout = new QVBoxLayout();
    QScrollArea *scrollArea = new QScrollArea();

    ibuttonsLayout->addWidget(cAddInventor);
    for(auto i=0; i < cInventorButtons.size(); i++){
        ibuttonsLayout->addWidget(cInventorButtons[i]);
    }

    setLayout(ibuttonsLayout);
    scrollArea->setWidget(this);

    QVBoxLayout *scrollLayout = new QVBoxLayout();
    scrollLayout->addWidget(scrollArea);

    groupBox->setLayout(scrollLayout);

    QGridLayout *frameLayout = new QGridLayout();
    frameLayout->addWidget(groupBox, 0, 0);
    setLayout(frameLayout);
}

void InventorsPanel::ConnectSignalsAndSlots()
{
    QObject::connect(cAddInventor,  &AddNewButton::clicked,
                     this,           &InventorsPanel::AddInventor);

    for(int i=0; i < cInventorButtons.size(); i++){
        QObject::connect(cInventorButtons[i],   &InventorButton::signalOpenInventor,
                         this,                   &InventorsPanel::OpenInventor);
    }

    for(int i=0; i < cInventorButtons.size(); i++){
        QObject::connect(cInventorButtons[i],   &InventorButton::signalRemoveInventor,
                        this,                   &InventorsPanel::RemoveInventor);
    }
}

} // namespace Nutmeg
