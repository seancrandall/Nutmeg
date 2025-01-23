#include "inventorcompleter.h"

namespace Nutmeg
{

InventorCompleter::InventorCompleter(QObject *parent) : Nutmeg::Completer{parent}
{
    mod = new viewInventorsModel(this);
    setModel(mod);
    setCompletionColumn(3);
}

} // namespace Nutmeg
