#include "viewworkattorneysmodel.h"

namespace Nutmeg
{

viewWorkAttorneysModel::viewWorkAttorneysModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("viewWorkAttorneys");
    select();
    mIsLoaded = true;
    IndexLocations();
}

} // namespace Nutmeg
