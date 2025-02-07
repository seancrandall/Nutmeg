#include "viewpatentmattersmodel.h"

namespace Nutmeg {

viewPatentMattersModel::viewPatentMattersModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewPatentMatters");
    if(select())
    {
        mIsLoaded = true;
        IndexLocations();
    }
}

} // namespace Nutmeg
