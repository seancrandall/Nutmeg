#ifndef MATTERPANEL_H
#define MATTERPANEL_H

#include <QObject>
#include <QWidget>

#include "nutmeg.h"
#include "frame.h"
#include "property.h"
#include "objects/matter.h"
#include "objects/person.h"
#include "objects/entity.h"
#include "objects/enterprise.h"
#include "widgets/groupbox.h"
#include "widgets/mattersearchbox.h"
#include "dbaccess/viewclientsmodel.h"
#include "dbaccess/viewcontractingfirmsmodel.h"
#include "dbaccess/viewjurisdictionsmodel.h"
#include "dbaccess/viewparalegalsmodel.h"
#include "dbaccess/viewworkattorneysmodel.h"
#include "widgets/arrowbutton.h"
#include "widgets/examinereditadd.h"
#include "widgets/inventoreditadd.h"
#include "widgets/labeledwidgetleft.h"
#include "widgets/linedisplayid.h"
#include "widgets/paralegalsearchbox.h"
#include "widgets/titleedit.h"
#include "widgets/workattorneysearchbox.h"

namespace Nutmeg {

//class MatterPanel : public Nutmeg::Frame
class MatterPanel : public Nutmeg::GroupBox
{
    Q_OBJECT

public:
    MatterPanel(std::shared_ptr<Matter> matterin, QWidget *parent = nullptr);
    MatterPanel(Key matterid, QWidget *parent = nullptr);

public slots:
    void scatter(void);
    void gather(void);

protected:
    void initializeObjects(void);
    void setupLayouts(void);

    // Layouts
    QVBoxLayout *vone = new QVBoxLayout();
    QGridLayout *grid, *fullLayout;

    // Labels
    LabeledWidgetLeft *lId, *lDocketNumber, *lParent, *lClientDocket, *lClient, *lTitle, *lAssigningFirm,
        *lDefaultWorkAttorney, *lDefaultParalegal, *lJurisdiction;

    // Controls
    LineDisplayId *cId;
    LineEdit *cDocketNumber;
    MatterSearchBox *cParent;
    LineEdit *cClientDocket;
    ComboBox *cClient;
    TitleEdit *cTitle;
    ComboBox *cAssigningFirm;
    WorkAttorneySearchBox *cDefaultWorkAttorney;
    ParalegalSearchBox *cDefaultParalegal;
    ComboBox *cJurisdiction;

    std::shared_ptr<Matter> matter;

private:
    void ConnectSignalsAndSlots(void);
};

} // namespace Nutmeg

#endif // MATTERPANEL_H
