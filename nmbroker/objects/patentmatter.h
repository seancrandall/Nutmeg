#ifndef NUTMEG_PATENTMATTER_H
#define NUTMEG_PATENTMATTER_H

#include "matter.h"

#define patentMatterTableName "patentMatter"

namespace Nutmeg
{

class PatentMatter : public Nutmeg::Matter
{
  public:
    explicit PatentMatter();
      explicit PatentMatter(Key id);
    explicit PatentMatter(String docketNumber);

    Property(getId, SetId) Key PatentMatterId;
    Property(getFilingDate, SetFilingDate) Date FilingDate;
    Property(getApplicationSerialNumber, SetApplicationSerialNumber) String ApplicationSerialNumber;
    Property(getConfirmationNumber, SetConfirmationNumber) String ConfirmationNumber;
    Property(getArtUnit, SetArtUnit) String ArtUnit;
    Property(getPatentNumber, SetPatentNumber) String PatentNumber;
    Property(getfkExaminer, SetfkExaminer) Key fkExaminer;
    Property(getfkFirstInventor, SetfkFirstInventor) Key fkFirstInventor;
    Property(getfkSupervisoryExaminer, SetfkSupervisoryExaminer) Key fkSupervisoryExaminer;
    Property(getfkApplicant, SetfkApplicant) Key fkApplicant;
    Property(getBarDate, SetBarDate) Date BarDate;
    Property(getCriticalDate, SetCriticalDate) Date CriticalDate;
    Property(getfkSpecification, SetfkSpecification) Key fkSpecification;
    Property(getfkDrawings, SetfkDrawings) Key fkDrawings;
    Property(getfkAsFiledClaims, SetfkAsFiledClaims) Key fkAsFiledClaims;
    ReadOnlyProperty(getInventors) QList<Key> inventors;

    // Getters
    virtual Key getId(void) const override { return mDat.PatentMatterId; }
    Date getFilingDate(void) { return mDat.FilingDate; }
    String getApplicationSerialNumber(void) { return mDat.ApplicationSerialNumber; }
    String getConfirmationNumber(void) { return mDat.ConfirmationNumber; }
    String getArtUnit(void) { return mDat.ArtUnit; }
    String getPatentNumber(void) { return mDat.PatentNumber; }
    Key getfkExaminer(void) { return mDat.fkExaminer; }
    Key getfkFirstInventor(void) { return mDat.fkFirstInventor; }
    Key getfkSupervisoryExaminer(void) { return mDat.fkSupervisoryExaminer; }
    Key getfkApplicant(void) { return mDat.fkApplicant; }
    Date getBarDate(void) { return mDat.BarDate; }
    Date getCriticalDate(void) { return mDat.CriticalDate; }
    Key getfkSpecification(void) { return mDat.fkSpecification; }
    Key getfkDrawings(void) { return mDat.fkDrawings; }
    Key getfkAsFiledClaims(void) { return mDat.fkAsFiledClaims; }

    QList<Key> getInventors(void);

    bool AddInventor(QString first, QString last);
    bool AddInventors(QList<PersonData> inventors);
    bool AddExaminer(String first, String last);
    bool AddSupervisoryExaminer(String first, String last);

    bool Update(PatentMatterData dat);
    virtual bool SetId(Key newval) override;
    virtual bool Commit(void) override;

    bool SetFilingDate(Date newval);
    bool SetApplicationSerialNumber(String newval);
    bool SetConfirmationNumber(String newval);
    bool SetArtUnit(String newval);
    bool SetPatentNumber(String newval);
    bool SetfkExaminer(Key newval);
    bool SetfkFirstInventor(Key newval);
    bool SetfkSupervisoryExaminer(Key newval);
    bool SetfkApplicant(Key newval);
    bool SetBarDate(Date newval);
    bool SetCriticalDate(Date newval);
    bool SetfkSpecification(Key newval);
    bool SetfkDrawings(Key newval);
    bool SetfkAsFiledClaims(Key newval);

    void holdFilingDate(Date newval) {mDat.FilingDate = newval;}
    void holdApplicationSerialNumber(String newval) {mDat.ApplicationSerialNumber = newval;}
    void holdConfirmationNumber(String newval) {mDat.ConfirmationNumber = newval;}
    void holdArtUnit(String newval) {mDat.ArtUnit = newval;}
    void holPatentNumber(String newval) {mDat.PatentNumber = newval;}
    void holdfkExaminer(Key newval) {mDat.fkExaminer = newval;}
    void holdfkFirstInventor(Key newval) {mDat.fkFirstInventor = newval;}
    void holdfkSupervisoryExaminer(Key newval) {mDat.fkSupervisoryExaminer = newval;}
    void holdfkApplicant(Key newval) {mDat.fkApplicant = newval;}
    void holdBarDate(Date newval) {mDat.BarDate = newval;}
    void holdCriticalDate(Date newval) {mDat.CriticalDate = newval;}
    void holdfkSpecification(Key newval) {mDat.fkSpecification = newval;}
    void holdfkDrawings(Key newval) {mDat.fkDrawings = newval;}
    void holdfkAsFiledClaims(Key newval) {mDat.fkAsFiledClaims = newval;}


protected:
    bool InitializePatentMatter(Key id);

    PatentMatterData mDat;
    QList<Key> mInventors;

private:
    static PatentMatter* GetPatentMatter(Key id);
};

} // namespace Nutmeg

#endif // NUTMEG_PATENTMATTER_H
