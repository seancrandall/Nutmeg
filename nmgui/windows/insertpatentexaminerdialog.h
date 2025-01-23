#ifndef NUTMEG_INSERTPATENTEXAMINERDIALOG_H
#define NUTMEG_INSERTPATENTEXAMINERDIALOG_H

#include "insertpersondialog.h"
#include <QObject>
#include <QWidget>

namespace Nutmeg
{

class InsertPatentExaminerDialog : public Nutmeg::InsertPersonDialog
{
    Q_OBJECT
  public:
    explicit InsertPatentExaminerDialog(QWidget *parent = nullptr);

  public slots:
    void slotHandleAccepted(void);
};

} // namespace Nutmeg

#endif // NUTMEG_INSERTPATENTEXAMINERDIALOG_H
