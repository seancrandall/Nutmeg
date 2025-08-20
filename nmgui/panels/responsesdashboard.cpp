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
    gResponsesDashboardComplete->select();

    // Populate the responses container with ResponsePanels
    auto rows = gResponsesDashboardComplete->rowCount();
    for (auto i = 0; i < rows; i++)
    {
        //responsesDashboardEntry entry = gResponsesDashboardComplete->record[i];
        QSqlRecord rec = gResponsesDashboardComplete->record(i);
        responsesDashboardEntry entry(rec);
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
