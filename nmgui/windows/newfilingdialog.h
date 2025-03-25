#ifndef NEWFILINGDIALOG_H
#define NEWFILINGDIALOG_H

#include <QDialog>
#include <QWidget>

#include "objects/deadline.h"
#include "objects/filing.h"
#include "ui_newfilingdialog.h"
#include "windows/dialog.h"

#include "dbaccess/viewcopyrightfilingtypesmodel.h"
#include "dbaccess/viewfilingtypesmodel.h"
#include "dbaccess/viewpatentfilingtypesmodel.h"
#include "dbaccess/viewtaskclassmodel.h"
#include "dbaccess/viewtrademarkfilingtypesmodel.h"

namespace Ui
{
class NewFilingDialog;
}

namespace Nutmeg
{

class NewFilingDialog : public QDialog
{
    Q_OBJECT
  public:
    explicit NewFilingDialog(QWidget *parent = nullptr);
    ~NewFilingDialog();

  signals:
    void signalDialogDone(void);

  protected slots:
    void slotCreateMatter(void);
    void slotCreateTask(void);
    void slotTaskRejected(void);
    void slotUpdateTaskList(void);

  private slots:
    void slotDialogAccepted(void);
    void slotDialogRejected(void);

  private:
    Ui::NewFilingDialog *ui;

    void SetupCombos(void);
    void ConnectSignals(void);
    Nutmeg::TableModel *tasktypemodel;
};

} // namespace Nutmeg

#endif // NEWFILINGDIALOG_H
