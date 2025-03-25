#include "peoplecompleter.h"
#include "dbaccess/models.h"

namespace Nutmeg
{

PeopleCompleter::PeopleCompleter(QObject *parent) : Nutmeg::Completer{parent}
{
    if(!gViewPeopleModel)
        gViewPeopleModel = std::make_unique<viewPeopleModel>();
    mod = gViewPeopleModel.get();

    setModel(mod);
    setCompletionColumn(3);
}


} // namespace Nutmeg
