#include "mattercompleter.h"

namespace Nutmeg
{

MatterCompleter::MatterCompleter(QObject *parent) : Nutmeg::Completer{parent}
{
    mod = new viewMattersModel(this);
    setModel(mod);
    setCompletionColumn(4); // Column 4 is AttorneyDocketNumber
}

} // namespace Nutmeg
