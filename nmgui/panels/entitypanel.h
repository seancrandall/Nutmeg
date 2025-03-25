#ifndef ENTITYPANEL_H
#define ENTITYPANEL_H

#include <QGridLayout>

#include "panels/frame.h"
#include "objects/entity.h"
#include "widgets/combobox.h"
#include "widgets/labeledwidgetleft.h"
#include "widgets/linedisplayid.h"
#include "widgets/textedit.h"

namespace Nutmeg {

class EntityPanel : public Nutmeg::Frame
{
    Q_OBJECT
public:
    explicit EntityPanel(QWidget *parent=nullptr);
    explicit EntityPanel(Key entityId, QWidget *parent=nullptr);
    explicit EntityPanel(std::shared_ptr<Entity> entity, QWidget *parent=nullptr);

public slots:
    void scatter();
    void gather();

protected:
    std::shared_ptr<Entity> mEntity;

    LineDisplayId *cId;
    LineEdit *cName
            , *cFullLegalName
            , *cWebsite
            , *cPrimaryPhone
            , *cSecondaryPhone
            , *cFaxNumber
            , *cPrimaryEmail
            , *cSecondaryEmail
            , *cDocketPrefix
            ;

    TextEdit *cPrimaryAddress
        , *cSecondaryAddress;

    //ComboBox *cJurisdiction
         //, *cState;
           //;

    LabeledWidgetLeft *lId
                    , *lName
                    , *lFullLegalName
                    , *lPrimaryAddress
                    , *lSecondaryAddress
                    , *lWebsite
                    , *lPrimaryPhone
                    , *lSecondaryPhone
                    , *lFaxNumber
                    , *lPrimaryEmail
                    , *lSecondaryEmail
                    //, *lState
                    //, *lJurisdiction
                    , *lDocketPrefix
                    ;

private:
    void Initialize(void);
    void InitializeObjects(void);
    void LayoutControls(void);
};

} // namespace Nutmeg

#endif // ENTITYPANEL_H
