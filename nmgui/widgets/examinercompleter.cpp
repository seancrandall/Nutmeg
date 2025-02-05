#include "examinercompleter.h"
#include "dbaccess/models.h"

namespace Nutmeg
{

ExaminerCompleter::ExaminerCompleter(QObject *parent) : Nutmeg::Completer{parent}
{
    //mod = new viewPatentExaminersModel(this);

    if(!gViewPatentExaminersModel)
        gViewPatentExaminersModel = std::make_unique<viewPatentExaminersModel>();
    mod = gViewPatentExaminersModel.get();

    setModel(mod);
    setCompletionColumn(3);
}

} // namespace Nutmeg
