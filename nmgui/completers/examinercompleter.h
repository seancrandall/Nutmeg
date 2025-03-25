#ifndef NUTMEG_EXAMINERCOMPLETER_H
#define NUTMEG_EXAMINERCOMPLETER_H

#include <QObject>
#include <QWidget>

#include "completer.h"
#include "dbaccess/viewpatentexaminersmodel.h"

namespace Nutmeg
{

class ExaminerCompleter : public Nutmeg::Completer
{
    Q_OBJECT
  public:
    explicit ExaminerCompleter(QObject *parent = nullptr);

  private:
    viewPatentExaminersModel *mod;
};

} // namespace Nutmeg

#endif // NUTMEG_EXAMINERCOMPLETER_H
