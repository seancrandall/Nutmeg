#ifndef NUTMEG_RESPONSEDASHPANEL_H
#define NUTMEG_RESPONSEDASHPANEL_H

#include <QHBoxLayout>
#include <QObject>
#include <QSqlDatabase>
#include <QWidget>
#include <QStackedWidget>
#include <QLabel>

#include "nutmeg.h"
#include "frame.h"
#include "objects/deadline.h"
#include "objects/matter.h"
#include "objects/response.h"
#include "objects/task.h"
#include "objects/responsesdashboardentry.h"

#include "panels/flagspanel.h"
#include "panels/entitiespanel.h"
#include "panels/responsetaskpanel.h"
#include "panels/deadlinespanel.h"

#include "widgets/combobox.h"
#include "widgets/donebutton.h"
#include "widgets/responsetypelabel.h"
#include "widgets/collapsebutton.h"
#include "widgets/docketnumberbutton.h"
#include "widgets/tasktypecombo.h"
#include "widgets/dateedit.h"
#include "widgets/dynamicstackedwidget.h"

namespace Nutmeg
{

class ResponseDashPanel : public Frame
{
    Q_OBJECT
public:
    explicit ResponseDashPanel(const responsesDashboardEntry &entry, QWidget *parent = nullptr);

public slots:
    void updateParalegal(Key newkey);
    void updateWorkAttorney(Key newkey);
    void updateClient(Key newkey);
    void expand();
    void collapse();

signals:
    void addNewResponse();
    void expanded();
    void collapsed();

protected slots:
    void changeResponseType();
    void changeNextDeadline();

protected:
    responsesDashboardEntry mEntry;
    std::shared_ptr<Deadline> mDeadline;
    std::shared_ptr<Response> mResponse;
    std::shared_ptr<Matter> mMatter;

    CollapseButton *collapseButton;
    DoneButton *doneButton;
    DynamicStackedWidget *stackedWidget;
    ResponseTypeLabel *responseTypeLabel;
    DocketNumberButton *docketNumberButton;
    TaskTypeCombo *taskTypeCombo;
    QLabel *nextDeadlineLabel;
    DateEdit *dateEdit;
    LabeledWidgetLeft *labeledDateEdit;
    ResponseTaskPanel *taskPanel;
    DeadlinesPanel *deadlinePanel;
    EntitiesPanel *entitiesPanel;
    FlagsPanel *flagsPanel;

private:
    void loadData();
    void connectSignalsAndSlots();
    void collapseButtonToggled();
};

} // namespace Nutmeg

#endif // NUTMEG_RESPONSEDASHPANEL_H
