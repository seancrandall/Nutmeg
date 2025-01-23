#ifndef NUTMEG_EXAMINEREDITADD_H
#define NUTMEG_EXAMINEREDITADD_H

#include "comboeditadd.h"
#include <QObject>
#include <QWidget>

namespace Nutmeg
{

class ExaminerEditAdd : public Nutmeg::ComboEditAdd
{
    Q_OBJECT
  public:
    ExaminerEditAdd(QWidget *parent = nullptr);
    ExaminerEditAdd(QString label, QWidget *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_EXAMINEREDITADD_H
