#ifndef NUTMEG_MAINWINDOW_H
#define NUTMEG_MAINWINDOW_H

#include <QGridLayout>
#include <QLayout>
#include <QMainWindow>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QSplashScreen>
#include <QPixmap>

#include "dbaccess/databaseconnection.h"
#include "nutmeg.h"
#include "settings.h"

#include "dbaccess/nutdb.h"
#include "dbaccess/viewfilingsincompletemodel.h"
#include "dbaccess/viewresponsesincompletemodel.h"

#include "panels/filingpanel.h"
#include "panels/responsepanel.h"

#include "widgets/addnewbutton.h"
#include "widgets/mattersearchbox.h"
#include "widgets/tableview.h"
#include "windows/newfilingdialog.h"
#include "windows/newresponsedialog.h"
#include "windows/optionswindow.h"

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
    void slotRefresh(void);
    void slotAddNewResponse(void);
    void slotAddNewFiling(void);
    void slotAddNewAppointment(void);

  private slots:

    void on_actionRefreshData_triggered(void) { slotRefresh(); }

    void on_actionPreferences_triggered();

  private:
    Ui::MainWindow *ui;

    void closeEvent(QCloseEvent *event);

    Nutmeg::Settings settings;
    Nutdb nut;

    // Filings Panel
    MatterSearchBox *matterSearch;

    // Responses Panel
    QList<ResponsePanel *> responsesPanel;
    viewResponsesIncompleteModel *responses;
    QLabel *responsesHeader;
    Nutmeg::AddNewButton *addRespButton;
    QHBoxLayout *responsesHeaderLayout;
    QScrollArea *responseScrollArea;
    QWidget *responsesContainer;
    QVBoxLayout *mainResponseLayout;

    // Filings Panel
    QList<FilingPanel *> filingsPanel;
    viewFilingsIncompleteModel *filings;
    QLabel *filingsHeader;
    Nutmeg::AddNewButton *addFilingButton;
    QHBoxLayout *filingsHeaderLayout;
    QScrollArea *filingsScrollArea;
    QWidget *filingsContainer;
    QVBoxLayout *mainFilingsLayout;

    TableView *apptsView;
    TableView *filingsView;
    // ResponsesView *responsesView;

    QGridLayout *apptsLayout;
    QVBoxLayout *filingsLayout;
    QVBoxLayout *responsesLayout;

    // Organization functions
    bool SetupDatabase(void);
    void SetupResponses(void);
    void SetupFilings(void);
    void SetupAppointments(void);
    void SetupHeader(void);

    bool databaseConnectionExists = false;
    std::shared_ptr<DatabaseConnection> dbconn;

    void ConnectSignalsAndSlots(void);
};

} // namespace Nutmeg
#endif // NUTMEG_MAINWINDOW_H
