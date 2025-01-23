#include "viewinventorsmodel.h"

namespace Nutmeg
{

viewInventorsModel::viewInventorsModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("viewInventors");
    select();
}

} // namespace Nutmeg
