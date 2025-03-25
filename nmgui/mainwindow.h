#ifndef NUTMEG_MAINWINDOW_H
#define NUTMEG_MAINWINDOW_H

#include <QGridLayout>
#include <QLayout>
#include <QMainWindow>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QSplashScreen>
#include <QPixmap>

#include "nutmeg.h"
#include "settings.h"

#include "dbaccess/nutdb.h"
#include "dbaccess/databaseconnection.h"

#include "windows/optionswindow.h"
#include "panels/maindashboard.h"

namespace Nutmeg
{

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  signals:
    void signalCreateNewResponse();

  protected slots:
    void Refresh(void);

  private slots:
    void on_actionRefreshData_triggered(void) { Refresh(); }
    void on_actionPreferences_triggered();

  private:
    Ui::MainWindow *ui;

    void closeEvent(QCloseEvent *event);

    Nutmeg::Settings settings;
    Nutdb nut;

    MainDashboard *dash;

    // Organization functions
    bool SetupDatabase(void);
    void SetupResponses(void);
    void SetupFilings(void);
    void SetupAppointments(void);

    bool databaseConnectionExists = false;
    std::shared_ptr<DatabaseConnection> dbconn;

    void ConnectSignalsAndSlots(void);
    void SetupDashboard(void);
};

} // namespace Nutmeg
#endif // NUTMEG_MAINWINDOW_H
