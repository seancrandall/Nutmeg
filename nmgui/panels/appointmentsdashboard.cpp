#include "appointmentsdashboard.h"

namespace Nutmeg {

AppointmentsDashboard::AppointmentsDashboard(QWidget *parent)
    : Frame(parent)
{
    SetupAppointments();
}

void AppointmentsDashboard::SetupAppointments()
{
    apptsHeader = new QLabel("Upcoming Appointments");
    apptsHeader->setFont((QFont("Arial", 14, QFont::Bold)));
    apptsHeader->setMaximumHeight(80);

    apptsHeaderLayout = new QHBoxLayout();
    apptsHeaderLayout->addWidget(apptsHeader);
    apptsHeaderLayout->addStretch();

    // Create scroll area
    apptsScrollArea = new QScrollArea(this);
    apptsScrollArea->setWidgetResizable(true);

    // Widget to hold the appointments
    apptsContainer = new Frame();
    apptsScrollArea->setWidget(apptsContainer);

    // Layout for the appointments container
    QVBoxLayout *apptsLayout = new QVBoxLayout(apptsContainer);

    gViewUpcomingAppointmentsModel = std::make_unique<viewUpcomingAppointmentsModel>();

    // Populate the appointments container
    for (auto i = 0; i < gViewUpcomingAppointmentsModel->rowCount(); i++)
    {
        Key id = gViewUpcomingAppointmentsModel->record(i).field(0).value().toUInt();
        AppointmentDashPanel *apanel = new AppointmentDashPanel(id, apptsContainer);
        apptDashPanels.append(apanel);
        apptsLayout->addWidget(apanel);
    }

    // Add scroll area to main layout
    mainApptsLayout = new QVBoxLayout();
    mainApptsLayout->addLayout(apptsHeaderLayout);
    mainApptsLayout->addWidget(apptsScrollArea);
    setLayout(mainApptsLayout);
}


} // namespace Nutmeg
