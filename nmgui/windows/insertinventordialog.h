#ifndef NUTMEG_INSERTINVENTORDIALOG_H
#define NUTMEG_INSERTINVENTORDIALOG_H

#include "insertpersondialog.h"
#include <QObject>
#include <QWidget>

namespace Nutmeg
{

class InsertInventorDialog : public Nutmeg::InsertPersonDialog
{
    Q_OBJECT
  public:
    explicit InsertInventorDialog(QWidget *parent = nullptr);

  public slots:
    virtual void slotHandleAccepted(void) override;
};

} // namespace Nutmeg

#endif // NUTMEG_INSERTINVENTORDIALOG_H
