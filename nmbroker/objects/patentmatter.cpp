#include "patentmatter.h"

namespace Nutmeg
{

PatentMatter::PatentMatter(QObject *parent) : Nutmeg::Matter{parent} {}

PatentMatter::PatentMatter(Key id, QObject *parent) : Nutmeg::Matter(id, parent)
{
    InitializePatentMatter(id);
}

PatentMatter::PatentMatter(String docketNumber, QObject *parent) : Nutmeg::Matter(parent)
{
    Nutdb::InsertPatentMatter(docketNumber);
}

QList<Key> PatentMatter::getInventors()
{
    mInventors = QList<Key>();
    mInventors = Nutdb::GetCaseInventors(mDat.PatentMatterId);
    return mInventors;
}

bool PatentMatter::slotSetFilingDate(Date newval)
{
    bool result = WriteDate(patentMatterTableName, "FilingDate", newval);
    if(result) mDat.FilingDate = newval;
    return result;
}

bool PatentMatter::slotSetApplicationSerialNumber(String newval)
{
    bool result = WriteString(patentMatterTableName, "ApplicationSerialNumber", newval);
    if(result) mDat.ApplicationSerialNumber = newval;
    return result;
}

bool PatentMatter::slotSetConfirmationNumber(String newval)
{
    bool result = WriteString(patentMatterTableName, "ConfirmationNumber", newval);
    if(result) mDat.ConfirmationNumber = newval;
    return result;
}

bool PatentMatter::slotSetArtUnit(String newval)
{
    bool result = WriteValue(patentMatterTableName, "ArtUnit", newval);
    if(result) mDat.ArtUnit = newval;
    return result;
}

bool PatentMatter::slotSetPatentNumber(String newval)
{
    bool result = WriteString(patentMatterTableName, "PatentNumber", newval);
    if(result) mDat.PatentNumber = newval;
    return result;
}

bool PatentMatter::slotSetfkExaminer(Key newval)
{
    bool result = WriteKey(patentMatterTableName, "fkExaminer", newval);
    if(result) mDat.fkExaminer = newval;
    return result;
}

bool PatentMatter::slotSetfkFirstInventor(Key newval)
{
    bool result = WriteKey(patentMatterTableName, "fkFirstInventor", newval);
    if(result) mDat.fkFirstInventor = newval;
    return result;
}

bool PatentMatter::slotSetfkSupervisoryExaminer(Key newval)
{
    bool result = WriteKey(patentMatterTableName, "fkSupervisoryExaminer", newval);
    if(result) mDat.fkSupervisoryExaminer = newval;
    return result;
}

bool PatentMatter::slotSetfkApplicant(Key newval)
{
    bool result = WriteKey(patentMatterTableName, "fkApplicant", newval);
    if(result) mDat.fkApplicant = newval;
    return result;
}

bool PatentMatter::slotSetBarDate(Date newval)
{
    bool result = WriteDate(patentMatterTableName, "BarDate", newval);
    if(result) mDat.BarDate = newval;
    return result;
}

bool PatentMatter::slotSetCriticalDate(Date newval)
{
    bool result = WriteDate(patentMatterTableName, "CriticalDate", newval);
    if(result) mDat.CriticalDate = newval;
    return result;
}

bool PatentMatter::slotSetfkSpecification(Key newval)
{
    bool result = WriteKey(patentMatterTableName, "fkSpecification", newval);
    if(result) mDat.fkSpecification = newval;
    return result;
}

bool PatentMatter::slotSetfkDrawings(Key newval)
{
    bool result = WriteKey(patentMatterTableName, "fkDrawings", newval);
    if(result) mDat.fkDrawings = newval;
    return result;
}

bool PatentMatter::slotSetfkAsFiledClaims(Key newval)
{
    bool result = WriteKey(patentMatterTableName, "fkAsFiledClaims", newval);
    if(result) mDat.fkAsFiledClaims = newval;
    return result;
}

bool PatentMatter::slotAddInventor(QString first, QString last)
{
    return Nutdb::InsertCaseInventor(first, last, mDat.PatentMatterId);
}

bool PatentMatter::slotAddInventors(QList<PersonData> inventors)
{
    for (auto i = 0; i < inventors.length(); i++)
    {
        if(!slotAddInventor(inventors[i].FirstName, inventors[i].LastName))
            return false;
    }
    return true;
}

bool PatentMatter::slotAddExaminer(String first, QString last)
{
    Key personid = Nutdb::InsertExaminer(first, last);
    if(personid == 0) return false;
    return slotSetfkExaminer(personid);
}

bool PatentMatter::slotAddSupervisoryExaminer(String first, String last)
{
    Key personid = Nutdb::InsertExaminer(first, last);
    if(personid == 0) return false;
    return slotSetfkSupervisoryExaminer(personid);
}

bool PatentMatter::slotUpdate(PatentMatterData dat)
{
    bool result = Nutdb::UpdatePatentMatter(dat);
    if(!result) return false;

    result = Matter::Commit();
    if(!result) return false;

    return InitializePatentMatter(dat.PatentMatterId);
}

bool PatentMatter::SetId(Key newval)
{
    return InitializePatentMatter(newval);
}

bool PatentMatter::Commit(void)
{
    return slotUpdate(mDat);
}

bool PatentMatter::InitializePatentMatter(Key id)
{
    if(id == 0)
    {
        mDat = PatentMatterData();
        return InitializeMatter(0);
    }

    mDat = Nutdb::GetPatentMatter(id);
    if(mDat.PatentMatterId == 0) return InitializePatentMatter(0);
    getInventors();
    return Matter::InitializeMatter(id);
}

} // namespace Nutmeg
