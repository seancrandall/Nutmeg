#ifndef NUTMEG_MATTERDIALOG_H
#define NUTMEG_MATTERDIALOG_H

#include "dialog.h"
#include "nutmeg.h"
#include "objects/matter.h"
#include "objects/object.h"
#include "property.h"
#include "widgets/groupbox.h"
#include "widgets/mattersearchbox.h"
#include <QObject>
#include <QWidget>

#include "dbaccess/viewclientsmodel.h"
#include "dbaccess/viewcontractingfirmsmodel.h"
#include "dbaccess/viewjurisdictionsmodel.h"
#include "dbaccess/viewparalegalsmodel.h"
#include "dbaccess/viewworkattorneysmodel.h"
#include "widgets/arrowbutton.h"
#include "widgets/examinereditadd.h"
#include "widgets/inventoreditadd.h"
#include "widgets/labeledwidgetleft.h"

namespace Nutmeg
{

class MatterDialog : public Nutmeg::Dialog
{
    Q_OBJECT
  public:
    MatterDialog(Key matterkey, QWidget *parent = nullptr);

  public slots:
    virtual void slotScatter(void) override;
    virtual void slotGather(void) override;

  protected:
    void setupDisplay(void) override;
    void connectSignalsAndSlots(void) override;
    void initializeObjects(void);
    void setupLayouts(void);
    void layoutWorkspace(void);

    // Layouts

    QVBoxLayout *vone = new QVBoxLayout();
    QGridLayout *grid = new QGridLayout();

    // Labels
    LabeledWidgetLeft *lId, *lDocketNumber, *lParent, *lClientDocket, *lClient, *lTitle, *lAssigningFirm,
        *lDefaultWorkAttorney, *lDefaultParalegal, *lJurisdiction;

    // Models
    viewParalegalsModel *modParalegals = new viewParalegalsModel(this);

    // Controls
    GroupBox *groupGeneralMatter = new GroupBox();
    LineDisplayId *cId;
    LineEdit *cDocketNumber;
    MatterSearchBox *cParent;
    LineEdit *cClientDocket;
    ComboBox *cClient;
    TitleEdit *cTitle;
    ComboBox *cAssigningFirm;
    ComboBox *cDefaultWorkAttorney;
    ComboBox *cDefaultParalegal;
    ComboBox *cJurisdiction;

    Object object;
    Matter matter;
};

} // namespace Nutmeg

#endif // NUTMEG_MATTERDIALOG_H
