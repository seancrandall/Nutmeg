#include "viewpeoplemodel.h"

namespace Nutmeg
{

viewPeopleModel::viewPeopleModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("viewPeople");
    select();
    mIsLoaded = true;
    IndexLocations();
}

} // namespace Nutmeg
