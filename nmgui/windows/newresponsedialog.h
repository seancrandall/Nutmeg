#ifndef NUTMEG_NEWRESPONSEDIALOG_H
#define NUTMEG_NEWRESPONSEDIALOG_H

#include <QDialog>

#include "ui_newresponsedialog.h"
#include "windows/dialog.h"

namespace Ui
{
class NewResponseDialog;
}

class NewResponseDialog : public QDialog
{
    Q_OBJECT

  public:
    explicit NewResponseDialog(QWidget *parent = nullptr);
    ~NewResponseDialog();

  signals:
    void signalDialogDone(void);

  protected slots:
    void slotCreateMatter(void);
    void slotCreateTask(void);
    void slotTaskRejected(void);
    void slotUpdateTaskList(void);

  private slots:

    void on_NewResponseDialog_accepted();

    void on_NewResponseDialog_rejected();

  private:
    Ui::NewResponseDialog *ui;

    void SetupCombos(void);
    void ConnectSignals(void);
    Nutmeg::TableModel *tasktypemodel;
};

#endif // NUTMEG_NEWMATTERDIALOG_H
