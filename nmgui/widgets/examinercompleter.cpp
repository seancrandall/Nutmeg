#include "examinercompleter.h"

namespace Nutmeg
{

ExaminerCompleter::ExaminerCompleter(QObject *parent) : Nutmeg::Completer{parent}
{
    mod = new viewPatentExaminersModel(this);
    setModel(mod);
    setCompletionColumn(3);
}

} // namespace Nutmeg
