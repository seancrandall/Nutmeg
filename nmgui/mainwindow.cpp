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
    ui->setupUi(this);

    // Restore window geometry
    setWindowTitle("Nutmeg—A Personal Docket Manager");

    //Restore geometry and state of the window
    restoreGeometry(settings.windowGeometry);
    restoreState(settings.windowState);

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
        // Create and show a splash screen
        QSplashScreen *splash = new QSplashScreen(QPixmap(), Qt::WindowStaysOnTopHint);
        splash->setAttribute(Qt::WA_DeleteOnClose); // Auto-delete when closed
        splash->show();
        splash->showMessage("Refreshing dashboard...", Qt::AlignCenter, Qt::white);

        // Ensure the splash screen is painted before the heavy work
        QApplication::processEvents();

        // Make sure models have the latest data
        gViewFilingsIncompleteModel->select();
        gResponsesDashboardComplete->select();
        gViewUpcomingAppointmentsModel->select();

        delete dash;
        SetupDashboard();

        // Hide and delete the splash screen
        splash->close();
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
    settings.windowGeometry = saveGeometry();
    settings.windowState = saveState();
    settings.splitterGeometry = dash->saveGeometry();
    settings.splitterState = dash->saveState();
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
    dash = new MainDashboard(this);
    setCentralWidget(dash);

    dash->restoreGeometry(settings.splitterGeometry);
    dash->restoreState(settings.splitterState);

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
