#include "entitiescompleter.h"

namespace Nutmeg
{

EntitiesCompleter::EntitiesCompleter(QObject *parent) : Nutmeg::Completer{parent}
{
    mod = new viewEntitiesModel(this);
    setModel(mod);
    setCompletionColumn(1);
}

} // namespace Nutmeg
