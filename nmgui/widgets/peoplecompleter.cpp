#include "peoplecompleter.h"

namespace Nutmeg
{

PeopleCompleter::PeopleCompleter(QObject *parent) : Nutmeg::Completer{parent}
{
    mod = new viewPeopleModel(this);
    setModel(mod);
    setCompletionColumn(3);
}

} // namespace Nutmeg
