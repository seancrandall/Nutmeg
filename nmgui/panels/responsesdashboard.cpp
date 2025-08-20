#include "responsesdashboard.h"
#include "windows/newresponsedialog.h"
#include "objects/responsesdashboardentry.h"
#include "dbaccess/models.h"

namespace Nutmeg {

ResponsesDashboard::ResponsesDashboard(QWidget *parent)
    : Frame(parent)
{
    SetupResponses();
    ConnectSignalsAndSlots();
}

void ResponsesDashboard::SetupResponses()
{
    // Setup header
    responsesHeader = new QLabel("Responses/Prosecution Tasks");
    responsesHeader->setFont((QFont("Arial", 14, QFont::Bold)));
    responsesHeader->setMaximumHeight(80);
    addRespButton = new Nutmeg::AddNewButton();
    responsesHeaderLayout = new QHBoxLayout();
    // Add widgets to layout
    responsesHeaderLayout->addWidget(responsesHeader);
    responsesHeaderLayout->addWidget(addRespButton);
    responsesHeaderLayout->addStretch();

    // Create a scroll area
    responseScrollArea = new QScrollArea(this);
    responseScrollArea->setWidgetResizable(true);

    // Create a widget to hold the responses and set it as the scroll area's
    // widget
    responsesContainer = new Frame();
    responseScrollArea->setWidget(responsesContainer);

    // Create a layout for the responses container
    QVBoxLayout *responsesLayout = new QVBoxLayout(responsesContainer);

    // Create the global responses model if it hasn't already been loaded
    gResponsesDashboardComplete = std::make_unique<ResponsesDashboardComplete>();

    // Populate the responses container with ResponsePanels
    auto rows = gResponsesDashboardComplete->rowCount();
    for (auto i = 0; i < rows; i++)
    {
        QSqlRecord rec = gResponsesDashboardComplete->record(i);
        responsesDashboardEntry entry;
        entry.slotSetTaskId(rec.field("TaskId").value().toUInt());
        entry.slotSetTaskClassName(rec.field("TaskClassName").value().toString());
        entry.slotSetAttorneyDocketNumber(rec.field("AttorneyDocketNumber").value().toString());
        entry.slotSetTaskName(rec.field("TaskName").value().toString());
        entry.slotSetTitle(rec.field("Title").value().toString());
        entry.slotSetTriggerDate(rec.field("TriggerDate").value().toDate());
        entry.slotSetNextDeadline(rec.field("NextDeadline").value().toDate());
        entry.slotSetSoftDeadline(rec.field("SoftDeadline").value().toDate());
        entry.slotSetHardDeadline(rec.field("HardDeadline").value().toDate());
        entry.slotSetClientEntityId(rec.field("ClientEntityId").value().toUInt());
        entry.slotSetClientEntityName(rec.field("ClientEntityName").value().toString());
        entry.slotSetParalegalEntityName(rec.field("ParalegalEntityName").value().toString());
        entry.slotSetWorkAttorneyEntityName(rec.field("WorkAttorneyEntityName").value().toString());
        entry.slotSetWithParalegal(rec.field("WithParalegal").value().toBool());
        entry.slotSetNeedsExaminerInterview(rec.field("NeedsExaminerInterview").value().toBool());
        entry.slotSetExaminerInterviewScheduled(rec.field("ExaminerInterviewScheduled").value().toBool());
        ResponseDashPanel *rpanel = new ResponseDashPanel(entry, responsesContainer);
        responseDashPanels.append(rpanel);
        responsesLayout->addWidget(rpanel);
    }

    // Add the scroll area to the main layout
    mainResponseLayout = new QVBoxLayout(this);
    mainResponseLayout->addLayout(responsesHeaderLayout);
    mainResponseLayout->addWidget(responseScrollArea);
    setLayout(mainResponseLayout);
}

void ResponsesDashboard::ConnectSignalsAndSlots()
{
    QObject::connect(addRespButton, &Nutmeg::AddNewButton::clicked,
                     this, &ResponsesDashboard::AddNewResponse);
}

void ResponsesDashboard::AddNewResponse()
{
    // ConnectSignalsAndSlots();
    NewResponseDialog nrdialog;
    nrdialog.setModal(true);
    bool result = nrdialog.exec();
    if(result)
        emit signalRefresh();
}

} // namespace Nutmeg
