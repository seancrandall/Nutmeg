#include "viewjurisdictionsmodel.h"

namespace Nutmeg
{

viewJurisdictionsModel::viewJurisdictionsModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("viewJurisdictions");
    select();
}

} // namespace Nutmeg
