#ifndef PERSONADDSELECTPANEL_H
#define PERSONADDSELECTPANEL_H

#include "widgets/personsearchbox.h"
#include "panels/insertpersonpanel.h"
#include "widgets/plusbutton.h"

#include "frame.h"
#include "property.h"

namespace Nutmeg {

class PersonAddSelectPanel : public Nutmeg::Frame
{
    Q_OBJECT
public:
    PersonAddSelectPanel(QWidget *parent = nullptr);

    Property(getFirstName, setFirstName) QString FirstName;
    Property(getLastName, setLastName) QString LastName;

    const QString getFirstName(void) const;
    const QString getLastName(void) const;

    void setFirstName(const QString& newName);
    void setLastName(const QString& lastName);

signals:
    void PersonSelected(Key selection);

public slots:
    Key accepted(void);
    void rejected(void);

    InsertPersonPanel *cInsertPerson;
    PersonSearchBox *cSearchPerson;
    PlusButton *cPlusButton;

private:
    void Initialize();
    void InitializeObjects();
    void Layout();
    void ConnectSignalsAndSlots();

    bool makeNew = false;

private slots:
    void toggleNewPerson(void);
};

} // namespace Nutmeg

#endif // PERSONADDSELECTPANEL_H
