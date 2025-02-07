#include "mattercompleter.h"
#include "dbaccess/models.h"

namespace Nutmeg
{

MatterCompleter::MatterCompleter(QObject *parent) : Nutmeg::Completer{parent}
{
    //mod = new viewMattersModel(this);
    if(!gViewMattersModel)
        gViewMattersModel = std::make_unique<viewMattersModel>();
    mod = gViewMattersModel.get();
    setModel(mod);
    setCompletionColumn(4); // Column 4 is AttorneyDocketNumber
}

} // namespace Nutmeg
