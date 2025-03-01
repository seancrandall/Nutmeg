#include "maindashboard.h"

namespace Nutmeg {


MainDashboard::MainDashboard(QWidget *parent)
    : Frame(parent)
    , splitter(new QSplitter())
{
    splitter->setOrientation(Qt::Vertical);
    splitter->setChildrenCollapsible(true);

    Setup();
}

QByteArray MainDashboard::saveState()
{
    return splitter->saveState();
}

bool MainDashboard::restoreState(const QByteArray &state)
{
    return splitter->restoreState(state);
}

QByteArray MainDashboard::saveGeometry()
{
    return splitter->saveGeometry();
}

bool MainDashboard::restoreGeometry(const QByteArray &geometry)
{
    return splitter->restoreGeometry(geometry);
}

void MainDashboard::Refresh()
{
    emit signalRefresh();
}

void MainDashboard::Setup()
{
    appts = new AppointmentsDashboard();
    filings = new FilingsDashboard();
    resps = new ResponsesDashboard();

    splitter->addWidget(appts);
    splitter->addWidget(filings);
    splitter->addWidget(resps);

    //Layout splitter as the sole widget
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(splitter);
    setLayout(layout);

    QObject::connect(appts,     &AppointmentsDashboard::signalRefresh,
                     this,       &MainDashboard::Refresh);

    QObject::connect(filings,   &FilingsDashboard::signalRefresh,
                     this,       &MainDashboard::Refresh);

    QObject::connect(resps,     &ResponsesDashboard::signalRefresh,
                     this,       &MainDashboard::Refresh);
}

} // namespace Nutmeg
