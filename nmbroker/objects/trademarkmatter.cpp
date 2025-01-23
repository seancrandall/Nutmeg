#include "trademarkmatter.h"

namespace Nutmeg
{

TrademarkMatter::TrademarkMatter(QObject *parent) : Nutmeg::Matter{parent} {}

TrademarkMatter::TrademarkMatter(Key id, QObject *parent) : Nutmeg::Matter(id, parent)
{
    InitializeTrademarkMatter(id);
}

TrademarkMatter::TrademarkMatter(QString docketNumber, QObject *parent) : Nutmeg::Matter(parent)
{
    Key id = Nutdb::InsertTrademarkMatter(docketNumber);
    if(id != 0)
        InitializeTrademarkMatter(id);
    else
        InitializeTrademarkMatter(0);
}

bool TrademarkMatter::slotSetFirstUseInCommerce(Date newval)
{
    bool result = WriteDate(trademarkMatterTableName, "FirstUseInCommerce", newval);
    if(result) mDat.FirstUseInCommerce = newval;
    return result;
}

bool TrademarkMatter::slotSetInternationalClass(int newval)
{
    bool result = WriteValue(trademarkMatterTableName, "InternationalClass", QVariant::fromValue(newval));
    if(result) mDat.InternationalClass = newval;
    return result;
}

bool TrademarkMatter::slotSetfkStatus(Key newval)
{
    bool result = WriteKey(trademarkMatterTableName, "fkStatus", newval);
    if(result) mDat.fkStatus = newval;
    return result;
}

bool TrademarkMatter::slotSetSerialNumber(String newval)
{
    bool result = WriteString(trademarkMatterTableName, "SerialNumber", newval);
    if(result) mDat.SerialNumber = newval;
    return result;
}

bool TrademarkMatter::slotSetRegistrationNumber(String newval)
{
    bool result = WriteString(trademarkMatterTableName, "RegistrationNumber", newval);
    if(result) mDat.RegistrationNumber = newval;
    return result;
}

bool TrademarkMatter::slotSetPublicationDate(Date newval)
{
    bool result = WriteDate(trademarkMatterTableName, "PublicationDate", newval);
    if(result) mDat.PublicationDate = newval;
    return result;
}

bool TrademarkMatter::slotSetWindowOpens(Date newval)
{
    bool result = WriteDate(trademarkMatterTableName, "WindoOpens", newval);
    if(result) mDat.WindowOpens = newval;
    return result;
}

bool TrademarkMatter::slotSetNofeeWindowCloses(Date newval)
{
    bool result = WriteDate(trademarkMatterTableName, "NoFeeWindowCloses", newval);
    if(result) mDat.NofeeWindowCloses = newval;
    return result;
}

bool TrademarkMatter::slotSetFinalWindowCloses(Date newval)
{
    bool result = WriteDate(trademarkMatterTableName, "NoFeeWindowCloses", newval);
    if(result) mDat.NofeeWindowCloses = newval;
    return result;
}

bool TrademarkMatter::slotSetfkTrademarkExaminer(Key newval)
{
    bool result = WriteKey(trademarkMatterTableName, "fkTrademarkExaminer", newval);
    if(result) mDat.fkTrademarkExaminer = newval;
    return result;
}

bool TrademarkMatter::slotSetfkFilingBasis(Key newval)
{
    bool result = WriteKey(trademarkMatterTableName, "fkFilingBasis", newval);
    if(result) mDat.fkFilingBasis = newval;
    return result;
}

bool TrademarkMatter::slotSetfkTrademarkJurisdiction(Key newval)
{
    bool result = WriteKey(trademarkMatterTableName, "fkTrademarkJurisdiction", newval);
    if(result) mDat.fkTrademarkJurisdiction = newval;
    return result;
}

bool TrademarkMatter::slotSetfkSpecimen(Key newval)
{
    bool result = WriteKey(trademarkMatterTableName, "fkSpecimen", newval);
    if(result) mDat.fkSpecimen = newval;
    return result;
}

bool TrademarkMatter::slotSetfkEvidenceOfUse(Key newval)
{
    bool result = WriteKey(trademarkMatterTableName, "fkEvidenceOfUse", newval);
    if(result) mDat.fkEvidenceOfUse = newval;
    return result;
}

bool TrademarkMatter::slotSetMark(String newval)
{
    bool result = WriteString(trademarkMatterTableName, "Mark", newval);
    if(result) mDat.Mark = newval;
    return result;
}

bool TrademarkMatter::slotSetGoodsServices(String newval)
{
    bool result = WriteString(trademarkMatterTableName, "GoodsServices", newval);
    if(result) mDat.GoodsServices = newval;
    return result;
}

bool TrademarkMatter::slotUpdate(TrademarkMatterData dat)
{
    bool result = Nutdb::UpdateTrademarkMatter(dat);
    if(!result) return false;
    result = Matter::slotCommit();
    if(!result) return false;

    return InitializeTrademarkMatter(dat.TrademarkMatterId);
}

bool TrademarkMatter::slotSetId(Key newval)
{
    return InitializeTrademarkMatter(newval);
}

bool TrademarkMatter::slotCommit()
{
   return slotUpdate(mDat);
}

bool TrademarkMatter::InitializeTrademarkMatter(Key newid)
{
    if(newid == 0)
    {
        mDat = TrademarkMatterData();
        return InitializeMatter(0);
    }

    mDat = Nutdb::GetTrademarkMatter(newid);
    if(mDat.TrademarkMatterId == 0) return InitializeTrademarkMatter(0);

    return Matter::InitializeMatter(mDat.TrademarkMatterId);
}

} // namespace Nutmeg
