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
    if(newkey) {
        mPatentMatter->AssignInventor(newkey);
        emit inventorsChanged();
    }
}

void InventorsPanel::RemoveInventor(Key inventorId)
{
    mPatentMatter->RemoveInventor(inventorId);
    emit inventorsChanged();
}

void InventorsPanel::OpenInventor(Key inventorId)
{
    EntityDialog diag(inventorId);
    diag.setModal(true);
    diag.exec();
}

void InventorsPanel::Initialize()
{
    clearLayout();
    LoadInventors();
    InitializeObjects();
    LayoutWidgets();
    ConnectSignalsAndSlots();
}

void InventorsPanel::clearLayout()
{
    if (layout() == nullptr) return; // Check if a layout exists

    QLayoutItem *item;
    while ((item = layout()->takeAt(0)) != nullptr) {
        if (item->widget()) {
            // No need to explicitly call removeWidget here since takeAt already removes it from the layout
            item->widget()->setParent(nullptr); // Optional, to ensure the widget isn't deleted when this InventorsPanel is destroyed
        }
        delete item; // Delete the layout item, not the widget
    }
    // Clear the buttons list to avoid dangling pointers
    cInventorButtons.clear();
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
    QWidget *scrollContent = new QWidget; // Container for scrollable content
    QScrollArea *scrollArea = new QScrollArea();

    // Set up the scrollable content
    for(auto i=0; i < cInventorButtons.size(); i++){
        ibuttonsLayout->addWidget(cInventorButtons[i]);
    }
    scrollContent->setLayout(ibuttonsLayout);

    // Configure scroll area
    scrollArea->setWidget(scrollContent);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Create a layout for the group box that includes both the scroll area and the Add New button
    QVBoxLayout *groupBoxLayout = new QVBoxLayout();
    groupBoxLayout->addWidget(scrollArea);
    groupBoxLayout->addWidget(cAddInventor); // Add New button outside the scroll area
    groupBox->setLayout(groupBoxLayout);

    // Set the layout for InventorsPanel
    QVBoxLayout *frameLayout = new QVBoxLayout(this);
    frameLayout->addWidget(groupBox);
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
