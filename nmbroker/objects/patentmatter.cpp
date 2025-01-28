#include "patentmatter.h"

namespace Nutmeg
{

PatentMatter::PatentMatter() : Nutmeg::Matter{} {}

PatentMatter::PatentMatter(Key id) : Nutmeg::Matter(id)
{
    PatentMatter* cachedPatentMatter = cache<PatentMatter>::getObjectFromCache(id, &PatentMatter::GetPatentMatter);
    if (cachedPatentMatter) {
        // If we find the PatentMatter in cache, copy its state
        *this = *cachedPatentMatter;
        mObjectIsNull = false; // Assuming this is inherited from Object/Matter
    } else {
        // If not in cache, proceed with initialization
        InitializePatentMatter(id);
    }
}

// Static method to fetch a PatentMatter from the database if not in cache
PatentMatter* PatentMatter::GetPatentMatter(Key id) {
    PatentMatter* patentMatter = new PatentMatter(id); // This will call the constructor again, but now with cache check
    if (patentMatter->mObjectIsNull) { // Assuming this is inherited from Object/Matter
        delete patentMatter; // Clean up if initialization failed
        return nullptr;
    }
    return patentMatter;
}

PatentMatter::PatentMatter(String docketNumber) : Nutmeg::Matter()
{
    Nutdb::InsertPatentMatter(docketNumber);
}

QList<Key> PatentMatter::getInventors()
{
    mInventors = QList<Key>();
    mInventors = Nutdb::GetCaseInventors(mDat.PatentMatterId);
    return mInventors;
}

bool PatentMatter::SetFilingDate(Date newval)
{
    bool result = WriteDate(patentMatterTableName, "FilingDate", newval);
    if(result) mDat.FilingDate = newval;
    return result;
}

bool PatentMatter::SetApplicationSerialNumber(String newval)
{
    bool result = WriteString(patentMatterTableName, "ApplicationSerialNumber", newval);
    if(result) mDat.ApplicationSerialNumber = newval;
    return result;
}

bool PatentMatter::SetConfirmationNumber(String newval)
{
    bool result = WriteString(patentMatterTableName, "ConfirmationNumber", newval);
    if(result) mDat.ConfirmationNumber = newval;
    return result;
}

bool PatentMatter::SetArtUnit(String newval)
{
    bool result = WriteValue(patentMatterTableName, "ArtUnit", newval);
    if(result) mDat.ArtUnit = newval;
    return result;
}

bool PatentMatter::SetPatentNumber(String newval)
{
    bool result = WriteString(patentMatterTableName, "PatentNumber", newval);
    if(result) mDat.PatentNumber = newval;
    return result;
}

bool PatentMatter::SetfkExaminer(Key newval)
{
    bool result = WriteKey(patentMatterTableName, "fkExaminer", newval);
    if(result) mDat.fkExaminer = newval;
    return result;
}

bool PatentMatter::SetfkFirstInventor(Key newval)
{
    bool result = WriteKey(patentMatterTableName, "fkFirstInventor", newval);
    if(result) mDat.fkFirstInventor = newval;
    return result;
}

bool PatentMatter::SetfkSupervisoryExaminer(Key newval)
{
    bool result = WriteKey(patentMatterTableName, "fkSupervisoryExaminer", newval);
    if(result) mDat.fkSupervisoryExaminer = newval;
    return result;
}

bool PatentMatter::SetfkApplicant(Key newval)
{
    bool result = WriteKey(patentMatterTableName, "fkApplicant", newval);
    if(result) mDat.fkApplicant = newval;
    return result;
}

bool PatentMatter::SetBarDate(Date newval)
{
    bool result = WriteDate(patentMatterTableName, "BarDate", newval);
    if(result) mDat.BarDate = newval;
    return result;
}

bool PatentMatter::SetCriticalDate(Date newval)
{
    bool result = WriteDate(patentMatterTableName, "CriticalDate", newval);
    if(result) mDat.CriticalDate = newval;
    return result;
}

bool PatentMatter::SetfkSpecification(Key newval)
{
    bool result = WriteKey(patentMatterTableName, "fkSpecification", newval);
    if(result) mDat.fkSpecification = newval;
    return result;
}

bool PatentMatter::SetfkDrawings(Key newval)
{
    bool result = WriteKey(patentMatterTableName, "fkDrawings", newval);
    if(result) mDat.fkDrawings = newval;
    return result;
}

bool PatentMatter::SetfkAsFiledClaims(Key newval)
{
    bool result = WriteKey(patentMatterTableName, "fkAsFiledClaims", newval);
    if(result) mDat.fkAsFiledClaims = newval;
    return result;
}

bool PatentMatter::AddInventor(QString first, QString last)
{
    return Nutdb::InsertCaseInventor(first, last, mDat.PatentMatterId);
}

bool PatentMatter::AddInventors(QList<PersonData> inventors)
{
    for (auto i = 0; i < inventors.length(); i++)
    {
        if(!AddInventor(inventors[i].FirstName, inventors[i].LastName))
            return false;
    }
    return true;
}

bool PatentMatter::AddExaminer(String first, QString last)
{
    Key personid = Nutdb::InsertExaminer(first, last);
    if(personid == 0) return false;
    return SetfkExaminer(personid);
}

bool PatentMatter::AddSupervisoryExaminer(String first, String last)
{
    Key personid = Nutdb::InsertExaminer(first, last);
    if(personid == 0) return false;
    return SetfkSupervisoryExaminer(personid);
}

bool PatentMatter::Update(PatentMatterData dat)
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
    return Update(mDat);
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
