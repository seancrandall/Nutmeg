#include "inventorspanel.h"
#include "objects/person.h"
#include "qgroupbox.h"
#include "qscrollarea.h"

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

void InventorsPanel::Initialize()
{
    LoadInventors();
    InitializeObjects();
    LayoutWidgets();
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

} // namespace Nutmeg
