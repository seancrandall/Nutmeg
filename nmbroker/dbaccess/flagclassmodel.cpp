#include "flagclassmodel.h"

namespace Nutmeg {

flagClassModel::flagClassModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("flagClass");
    if(select())
    {
        mIsLoaded = true;
        IndexLocations();
    }
}

} // namespace Nutmeg
