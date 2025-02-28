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
        SetupDashboard();
    }

}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::Refresh()
{
    if (databaseConnectionExists)
    {
        gViewFilingsIncompleteModel->select();
        gViewResponsesIncompleteModel->select();
        gViewUpcomingAppointmentsModel->select();
        delete dash;
        delete dashLayout;

        SetupDashboard();
    }
    else
    {
        QString noconnection = "No valid database connection, so nothing to refresh.";
        statusBar()->showMessage(noconnection);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    settings.setValue("geometry/window_geometry", saveGeometry());
    settings.setValue("geometry/window_state", saveState());
    settings.setValue("geometry/splitter_geometry", dash->saveGeometry());
    settings.setValue("geometry/splitter_state", dash->saveState());
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

void MainWindow::SetupDashboard()
{
    dash = new MainDashboard();
    dashLayout = new QGridLayout();
    dashLayout->addWidget(dash, 0, 0);
    ui->centralwidget->setLayout(dashLayout);
    QObject::connect(dash,      &MainDashboard::signalRefresh,
                     this,       &MainWindow::Refresh);
}

void MainWindow::on_actionPreferences_triggered()
{
    OptionsWindow opt(&settings, this);
    opt.setModal(true);
    opt.exec();
}

} // namespace Nutmeg
