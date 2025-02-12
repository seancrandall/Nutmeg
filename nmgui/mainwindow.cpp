#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widgets/addnewbutton.h"
#include "dbaccess/databaseconnection.h"
#include "dbaccess/models.h"

#include "windows/entitydialog.h"

namespace Nutmeg
{

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), settings(new Nutmeg::Settings(this)), nut(Nutmeg::Nutdb())

{

    // Restore splitter geometry
    ui->setupUi(this);
    ui->splitter->restoreState(settings.value("geometry/spliiter_state").toByteArray());

    // Restore window geometry
    setWindowTitle("Nutmeg—A Personal Docket Manager");
    restoreGeometry(settings.value("geometry/window_geometry").toByteArray());
    restoreState(settings.value("geometry/window_state").toByteArray());

    // Global Display stuff
    FontFamily = settings.fontFamily;
    FontSize = settings.fontSize;

    databaseConnectionExists = SetupDatabase();

    if (databaseConnectionExists)
    {
        SetupResponses();
        SetupFilings();
        SetupAppointments();
        ConnectSignalsAndSlots();
    }

}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::slotRefresh()
{
    if (databaseConnectionExists)
    {
        delete responsesHeaderLayout;
        delete responseScrollArea;
        delete mainResponseLayout;
        SetupResponses();

        delete filingsHeaderLayout;
        delete filingsScrollArea;
        delete mainFilingsLayout;
        SetupFilings();

        delete apptsHeaderLayout;
        delete apptsScrollArea;
        delete mainApptsLayout;
        SetupAppointments();

        ConnectSignalsAndSlots();
    }
    else
    {
        QString noconnection = "No valid database connection, so nothing to refresh.";
        statusBar()->showMessage(noconnection);
    }
}

void MainWindow::slotAddNewResponse()
{
    // ConnectSignalsAndSlots();
    NewResponseDialog nrdialog;
    nrdialog.setModal(true);
    bool result = nrdialog.exec();
    if (result)
        slotRefresh();
}

void MainWindow::slotAddNewFiling()
{
    NewFilingDialog nfdialog;
    nfdialog.setModal(true);
    bool result = nfdialog.exec();
    if (result)
        slotRefresh();
}

void MainWindow::slotAddNewAppointment() {}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    settings.setValue("geometry/window_geometry", saveGeometry());
    settings.setValue("geometry/window_state", saveState());
    settings.setValue("geometry/splitter_geometry", ui->splitter->saveGeometry());
    settings.setValue("geometry/splitter_state", ui->splitter->saveState());
}

bool MainWindow::SetupDatabase()
{
    dbconn = std::make_shared<DatabaseConnection>();
    dbconn->server = settings.server;
    dbconn->databasename = settings.databaseName;
    dbconn->port = settings.port;
    dbconn->username = settings.username;
    dbconn->password = settings.password;

    if (!dbconn->Connect())
    {
        qDebug() << "Failed to connect to database.";

        auto dbError = QString("Database connection failed with error: %1").arg(dbconn->lastError);
        statusBar()->showMessage(dbError);
        return false;
    }
    return true;
}

void MainWindow::SetupResponses()
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
    responseScrollArea = new QScrollArea(ui->frameBottom);
    responseScrollArea->setWidgetResizable(true);

    // Create a widget to hold the responses and set it as the scroll area's
    // widget
    responsesContainer = new QWidget();
    responseScrollArea->setWidget(responsesContainer);

    // Create a layout for the responses container
    responsesLayout = new QVBoxLayout(responsesContainer);

    // Create the global responses model if it hasn't already been loaded
    if(!gViewResponsesIncompleteModel)
        gViewResponsesIncompleteModel = std::make_unique<viewResponsesIncompleteModel>();

    // Populate the responses container with ResponsePanels
    auto rows = gViewResponsesIncompleteModel->rowCount();
    for (auto i = 0; i < rows; i++)
    {
        QSqlRecord fieldNames = gViewResponsesIncompleteModel->record();
        QSqlRecord rec = gViewResponsesIncompleteModel->record(i);
        Key id = rec.field(0).value().toUInt();
        ResponsePanel *rpanel = new ResponsePanel(id, responsesContainer);
        responsesPanel.append(rpanel);
        responsesLayout->addWidget(rpanel);
    }

    // Add the scroll area to the main layout
    mainResponseLayout = new QVBoxLayout(ui->frameBottom);
    mainResponseLayout->addLayout(responsesHeaderLayout);
    mainResponseLayout->addWidget(responseScrollArea);
}

void MainWindow::SetupFilings()
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
    filingsScrollArea = new QScrollArea(ui->frameMiddle);
    filingsScrollArea->setWidgetResizable(true);

    // Widget to hold the filings
    filingsContainer = new QWidget();
    filingsScrollArea->setWidget(filingsContainer);

    // Layout for the responses container
    filingsLayout = new QVBoxLayout(filingsContainer);

    // Filings model
    if(!gViewFilingsIncompleteModel)
        gViewFilingsIncompleteModel = std::make_unique<viewFilingsIncompleteModel>();

    // Populate the filings container
    for (auto i = 0; i < gViewFilingsIncompleteModel->rowCount(); i++)
    {
        Key id = gViewFilingsIncompleteModel->record(i).field(0).value().toUInt();
        FilingPanel *fpanel = new FilingPanel(id, filingsContainer);
        filingsPanel.append(fpanel);
        filingsLayout->addWidget(fpanel);
    }

    // Add scroll area to main layout
    mainFilingsLayout = new QVBoxLayout(ui->frameMiddle);
    mainFilingsLayout->addLayout(filingsHeaderLayout);
    mainFilingsLayout->addWidget(filingsScrollArea);
}

void MainWindow::SetupAppointments()
{
    apptsHeader = new QLabel("Upcoming Appointments");
    apptsHeader->setFont((QFont("Arial", 14, QFont::Bold)));
    apptsHeader->setMaximumHeight(80);

    apptsHeaderLayout = new QHBoxLayout();
    apptsHeaderLayout->addWidget(apptsHeader);
    apptsHeaderLayout->addStretch();

    // Create scroll area
    apptsScrollArea = new QScrollArea(ui->frameTop);
    apptsScrollArea->setWidgetResizable(true);

    // Widget to hold the appointments
    apptsContainer = new QWidget();
    apptsScrollArea->setWidget(apptsContainer);

    // Layout for the appointments container
    apptsLayout = new QVBoxLayout(apptsContainer);

    if(!gViewUpcomingAppointmentsModel)
        gViewUpcomingAppointmentsModel = std::make_unique<viewUpcomingAppointmentsModel>();

    // Populate the appointments container
    for (auto i = 0; i < gViewUpcomingAppointmentsModel->rowCount(); i++)
    {
        Key id = gViewUpcomingAppointmentsModel->record(i).field(0).value().toUInt();
        AppointmentDashPanel *apanel = new AppointmentDashPanel(id, apptsContainer);
        apptsPanel.append(apanel);
        apptsLayout->addWidget(apanel);
    }

    // Add scroll area to main layout
    mainApptsLayout = new QVBoxLayout(ui->frameTop);
    mainApptsLayout->addLayout(apptsHeaderLayout);
    mainApptsLayout->addWidget(apptsScrollArea);
}

void MainWindow::ConnectSignalsAndSlots()
{
    QObject::connect(addRespButton, &Nutmeg::AddNewButton::clicked,
                     this, &MainWindow::slotAddNewResponse);

    QObject::connect(addFilingButton, &Nutmeg::AddNewButton::clicked, this, &MainWindow::slotAddNewFiling);
}

void MainWindow::on_actionPreferences_triggered()
{
    OptionsWindow opt(&settings, this);
    opt.setModal(true);
    opt.exec();
}

} // namespace Nutmeg
