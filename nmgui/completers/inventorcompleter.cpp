#include "inventorcompleter.h"
#include "dbaccess/models.h"

namespace Nutmeg
{

InventorCompleter::InventorCompleter(QObject *parent) : Nutmeg::Completer{parent}
{
    //mod = new viewInventorsModel(this);
    if(!gViewInventorsModel)
        gViewInventorsModel = std::make_unique<viewInventorsModel>();
    mod = gViewInventorsModel.get();
    setModel(mod);
    setCompletionColumn(3);
}

} // namespace Nutmeg
