#ifndef NUTMEG_INSERTPERSONDIALOG_H
#define NUTMEG_INSERTPERSONDIALOG_H

#include <QObject>
#include <QWidget>

#include "dialog.h"
#include "windows/insertdialog.h"

#include "widgets/insertpersonpanel.h"

namespace Nutmeg
{

class InsertPersonDialog : public Nutmeg::InsertDialog
{
    Q_OBJECT
  public:
    explicit InsertPersonDialog(QWidget *parent = nullptr);
    explicit InsertPersonDialog(QString firstName, QString LastName, QWidget *parent = nullptr);

    InsertPersonPanel *ppanel = new InsertPersonPanel(this);

  public slots:
    virtual void slotHandleAccepted(void) override;
    virtual void slotHandleRejected(void) override;

  protected:
    virtual void connectSignalsAndSlots(void) override;
    virtual void setupDisplay(void) override;
};

} // namespace Nutmeg

#endif // NUTMEG_INSERTPERSONDIALOG_H
