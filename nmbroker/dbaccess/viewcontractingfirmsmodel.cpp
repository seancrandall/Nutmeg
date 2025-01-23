#include "viewcontractingfirmsmodel.h"

namespace Nutmeg
{

viewContractingFirmsModel::viewContractingFirmsModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("viewContractingFirms");
    select();
}

} // namespace Nutmeg
