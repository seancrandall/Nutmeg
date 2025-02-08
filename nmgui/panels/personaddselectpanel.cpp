#include "personaddselectpanel.h"

namespace Nutmeg {


PersonAddSelectPanel::PersonAddSelectPanel(QWidget *parent)
    : Frame(parent)
{
    Initialize();
}

const QString PersonAddSelectPanel::getFirstName() const
{
    return cInsertPerson->FirstName;
}

const QString PersonAddSelectPanel::getLastName() const
{
    return cInsertPerson->LastName;
}

void PersonAddSelectPanel::setFirstName(const QString &newName)
{
    cInsertPerson->FirstName = newName;
}

void PersonAddSelectPanel::setLastName(const QString &lastName)
{
    cInsertPerson->LastName = lastName;
}

Key PersonAddSelectPanel::accepted()
{
    Key returnkey = 0;
    if(makeNew){ //Make a new person
        returnkey = Nutdb::InsertPerson(cInsertPerson->FirstName, cInsertPerson->LastName);
        if(returnkey)
            emit PersonSelected(returnkey);
        else
            emit PersonSelected(0);
    } else {  //Select an existing person
        returnkey = cSearchPerson->key;
        emit PersonSelected(returnkey);
    }

    return returnkey;
}

void PersonAddSelectPanel::rejected()
{
    return;
}

void PersonAddSelectPanel::Initialize()
{
    InitializeObjects();
    Layout();
    ConnectSignalsAndSlots();
}

void PersonAddSelectPanel::InitializeObjects()
{
    cInsertPerson = new InsertPersonPanel();
    cSearchPerson = new PersonSearchBox();
    cPlusButton = new PlusButton();
}

void PersonAddSelectPanel::Layout()
{
    QGridLayout *layout = new QGridLayout();
    layout->addWidget(cSearchPerson, 0, 0);
    layout->addWidget(cPlusButton, 0, 1);
    layout->addWidget(cInsertPerson, 1, 0);

    cInsertPerson->hide();
    setLayout(layout);
}

void PersonAddSelectPanel::ConnectSignalsAndSlots()
{
    QObject::connect(cPlusButton,       &PlusButton::clicked,
                     this,               &PersonAddSelectPanel::toggleNewPerson);
}

void PersonAddSelectPanel::toggleNewPerson()
{
    if(makeNew){            //Switch to an existing person
        makeNew = false;
        cInsertPerson->hide();
        cPlusButton->setText("+");
    }  else {              //Switch to a new person
        makeNew = true;
        cInsertPerson->show();
        cPlusButton->setText("-");
    }
}

} // namespace Nutmeg
