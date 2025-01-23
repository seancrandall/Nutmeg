#ifndef NUTMEG_INSERTTRADEMARKEXAMINERDIALOG_H
#define NUTMEG_INSERTTRADEMARKEXAMINERDIALOG_H

#include "insertpersondialog.h"
#include <QObject>
#include <QWidget>

namespace Nutmeg
{

class InsertTrademarkExaminerDialog : public Nutmeg::InsertPersonDialog
{
    Q_OBJECT
  public:
    explicit InsertTrademarkExaminerDialog(QWidget *parent = nullptr);

  public slots:
    virtual void slotHandleAccepted(void) override;
};

} // namespace Nutmeg

#endif // NUTMEG_INSERTTRADEMARKEXAMINERDIALOG_H
