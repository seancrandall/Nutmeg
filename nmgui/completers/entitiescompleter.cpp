#include "entitiescompleter.h"
#include "dbaccess/models.h"

namespace Nutmeg
{

EntitiesCompleter::EntitiesCompleter(QObject *parent)
    : Nutmeg::Completer{parent}
{
    if(!gViewEntitiesModel)
        gViewEntitiesModel = std::make_unique<viewEntitiesModel>();

    mod = gViewEntitiesModel.get();

    setModel(mod);
    setCompletionColumn(1);
}

} // namespace Nutmeg
