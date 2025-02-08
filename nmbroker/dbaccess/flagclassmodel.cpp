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

    mFlagsByCamelCase = QHash<QString, Key>();
    //++i
    for(int i = 0; i < rowCount(); i++){
        QString camelCase = record(i).value("camelCase").toString();
        Key flagClassId = record(i).value("FlagClassId").toUInt();
        mFlagsByCamelCase[camelCase] = flagClassId;
    }
}

Key flagClassModel::getFlagByCamelCase(QString cc)
{
    if(!mFlagsByCamelCase.contains(cc)){
        return UINT32_MAX;
    }
    return mFlagsByCamelCase[cc];
}

} // namespace Nutmeg
