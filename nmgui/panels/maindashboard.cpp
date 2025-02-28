#include "maindashboard.h"

namespace Nutmeg {


MainDashboard::MainDashboard(QWidget *parent)
    : QSplitter(parent)
{
    setOrientation(Qt::Vertical);
    setChildrenCollapsible(true);

    Setup();
}

void MainDashboard::Setup()
{
    appts = new AppointmentsDashboard();
    filings = new FilingsDashboard();
    resps = new ResponsesDashboard();

    addWidget(appts);
    addWidget(filings);
    addWidget(resps);

    QObject::connect(appts,     &AppointmentsDashboard::signalRefresh,
                     this,       &MainDashboard::Refresh);

    QObject::connect(filings,   &FilingsDashboard::signalRefresh,
                     this,       &MainDashboard::Refresh);

    QObject::connect(resps,     &ResponsesDashboard::signalRefresh,
                     this,       &MainDashboard::Refresh);
}

} // namespace Nutmeg
