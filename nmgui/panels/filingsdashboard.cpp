#include "filingsdashboard.h"
#include "windows/newfilingdialog.h"

namespace Nutmeg {

FilingsDashboard::FilingsDashboard(QWidget *parent)
    : Frame(parent)
{
    SetupFilings();
}

void FilingsDashboard::AddNewFiling()
{
    NewFilingDialog nfdialog;
    nfdialog.setModal(true);
    bool result = nfdialog.exec();
    if (result)
        emit signalRefresh();
}

void FilingsDashboard::SetupFilings()
{
    // matterSearch = new MatterSearchBox(ui->frameMiddle);
    filingsHeader = new QLabel("Incomplete Filings");
    filingsHeader->setFont((QFont("Arial", 14, QFont::Bold)));
    filingsHeader->setMaximumHeight(80);
    addFilingButton = new Nutmeg::AddNewButton();
    filingsHeaderLayout = new QHBoxLayout();

    filingsHeaderLayout->addWidget(filingsHeader);
    filingsHeaderLayout->addWidget(addFilingButton);
    filingsHeaderLayout->addStretch();

    // Create scroll area
    filingsScrollArea = new QScrollArea(this);
    filingsScrollArea->setWidgetResizable(true);

    // Widget to hold the filings
    filingsContainer = new Frame();
    filingsScrollArea->setWidget(filingsContainer);

    // Layout for the responses container
    QVBoxLayout* filingsLayout = new QVBoxLayout(filingsContainer);

    // Filings model
    gViewFilingsIncompleteModel = std::make_unique<viewFilingsIncompleteModel>();

    // Populate the filings container
    for (auto i = 0; i < gViewFilingsIncompleteModel->rowCount(); i++)
    {
        Key id = gViewFilingsIncompleteModel->record(i).field(0).value().toUInt();
        FilingDashPanel *fpanel = new FilingDashPanel(id, filingsContainer);
        filingDashPanels.append(fpanel);
        filingsLayout->addWidget(fpanel);
    }

    // Add scroll area to main layout
    mainFilingsLayout = new QVBoxLayout(this);
    mainFilingsLayout->addLayout(filingsHeaderLayout);
    mainFilingsLayout->addWidget(filingsScrollArea);
}

void FilingsDashboard::ConnectSignalsAndSlots()
{
    QObject::connect(addFilingButton, &Nutmeg::AddNewButton::clicked,
                     this, &FilingsDashboard::AddNewFiling);
}

} // namespace Nutmeg
