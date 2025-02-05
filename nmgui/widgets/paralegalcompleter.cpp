#include "paralegalcompleter.h"
#include "dbaccess/models.h"

namespace Nutmeg {

ParalegalCompleter::ParalegalCompleter(QObject *parent)
    : Nutmeg::Completer{parent}
{
    if(!gViewParalegalsModel)
        gViewParalegalsModel = std::make_unique<viewParalegalsModel>();
    mod = gViewParalegalsModel.get();

    setModel(mod);
    setCompletionColumn(3);
}

} // namespace Nutmeg
