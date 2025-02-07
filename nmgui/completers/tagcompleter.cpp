#include "tagcompleter.h"
#include "dbaccess/models.h"

namespace Nutmeg {

TagCompleter::TagCompleter(QObject *parent)
    : Nutmeg::Completer{parent}
{
    if(!gTagModel)
        gTagModel = std::make_unique<tagModel>();

    mod = gTagModel.get();

    setModel(mod);
    setCompletionColumn(1);
}

} // namespace Nutmeg
