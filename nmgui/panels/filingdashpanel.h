#ifndef FILINGDASHPANEL_H
#define FILINGDASHPANEL_H

#include <QWidget>
#include <QHBoxLayout>

#include "nutmeg.h"
#include "frame.h"
#include "objects/deadline.h"
#include "objects/matter.h"
#include "objects/response.h"
#include "objects/task.h"

#include "widgets/combobox.h"
#include "widgets/donebutton.h"
#include "widgets/filingtypelabel.h"
#include "widgets/collapsebutton.h"         // Added for CollapseButton
#include "widgets/docketnumberbutton.h"     // Added for DocketNumberButton
#include "widgets/dateedit.h"               // Added for DateEdit
#include "widgets/labeledwidgetleft.h"      // Added for LabeledWidgetLeft
#include "widgets/dynamicstackedwidget.h"   // Added for DynamicStackedWidget
#include "panels/deadlinespanel.h"
#include "panels/filingtaskpanel.h"
#include "panels/entitiespanel.h"
#include "panels/flagspanel.h"

#include "dbaccess/viewclientsmodel.h"
#include "dbaccess/viewparalegalsmodel.h"
#include "dbaccess/viewworkattorneysmodel.h"

namespace Nutmeg
{

class FilingDashPanel : public Frame
{
    Q_OBJECT
public:
    explicit FilingDashPanel(Key id, QWidget *parent = nullptr);

public slots:
    void slotUpdateCompletion(void);
    void slotRefreshView(void);
    void EmailInventors(const QString& body);
    void expand();    // Added to programmatically expand the panel
    void collapse();  // Added to programmatically collapse the panel

signals:
    void signalAddNewFiling(void);
    void signalSomethingChanged(void);

protected:
    std::shared_ptr<Deadline> deadline;
    std::shared_ptr<Filing> filing;
    std::shared_ptr<Matter> matter;

    // Existing widgets
    DoneButton *doneButton;
    FilingTaskPanel *taskPanel;
    DeadlinesPanel *deadlinePanel;
    EntitiesPanel *entitiesPanel;
    FlagsPanel *flagsPanel;
    FilingTypeLabel *label;

    // New widgets for collapse feature
    CollapseButton *collapseButton;
    DynamicStackedWidget *stackedWidget;
    DocketNumberButton *docketNumberButton;
    TaskTypeCombo *taskCombo;
    DateEdit *dateEdit;
    LabeledWidgetLeft *labeledDateEdit;

private:
    void ConnectSignalsAndSlots(void);
    void Initialize(void);
    void SetColor(void);
    void InitializeControls(void);
    void LayoutWidgets(void);

private slots:
    void onCollapseButtonToggled();
    void updateDeadline();
    void updateFilingType();
};

} // namespace Nutmeg

#endif // FILINGDASHPANEL_H
