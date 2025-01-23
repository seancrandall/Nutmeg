#include "viewparalegalsmodel.h"

namespace Nutmeg
{

viewParalegalsModel::viewParalegalsModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("viewParalegals");
    select();
}

} // namespace Nutmeg
