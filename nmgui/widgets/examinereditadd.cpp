#include "examinereditadd.h"
#include "widgets/examinersearchbox.h"

namespace Nutmeg
{

ExaminerEditAdd::ExaminerEditAdd(QWidget *parent) : Nutmeg::ComboEditAdd(parent) {}

ExaminerEditAdd::ExaminerEditAdd(QString label, QWidget *parent) : ComboEditAdd(label, parent)
{
    delete cCombo;
    cCombo = new Nutmeg::ExaminerSearchBox();
    setupDisplay();
    connectSignalsAndSlots();
}

} // namespace Nutmeg
