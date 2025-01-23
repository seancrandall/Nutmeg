#ifndef NUTMEG_PATENTMATTER_H
#define NUTMEG_PATENTMATTER_H

#include "matter.h"
#include <QObject>

namespace Nutmeg
{

class PatentMatter : public Nutmeg::Matter
{
    Q_OBJECT
  public:
    explicit PatentMatter(QObject *parent = nullptr);
    explicit PatentMatter(Key id, QObject *parent = nullptr);
    explicit PatentMatter(String docketNumber, QObject *parent = nullptr);

    Property(getId, slotSetId) Key PatentMatterId;
    Property(getFilingDate, slotSetFilingDate) Date FilingDate;
    Property(getApplicationSerialNumber, slotSetApplicationSerialNumber) String ApplicationSerialNumber;
    Property(getConfirmationNumber, slotSetConfirmationNumber) String ConfirmationNumber;
    Property(getArtUnit, slotSetArtUnit) String ArtUnit;
    Property(getPatentNumber, slotSetPatentNumber) String PatentNumber;
    Property(getfkExaminer, slotSetfkExaminer) Key fkExaminer;
    Property(getfkFirstInventor, slotSetfkFirstInventor) Key fkFirstInventor;
    Property(getfkSupervisoryExaminer, slotSetfkSupervisoryExaminer) Key fkSupervisoryExaminer;
    Property(getfkApplicant, slotSetfkApplicant) Key fkApplicant;
    Property(getBarDate, slotSetBarDate) Date BarDate;
    Property(getCriticalDate, slotSetCriticalDate) Date CriticalDate;
    Property(getfkSpecification, slotSetfkSpecification) Key fkSpecification;
    Property(getfkDrawings, slotSetfkDrawings) Key fkDrawings;
    Property(getfkAsFiledClaims, slotSetfkAsFiledClaims) Key fkAsFiledClaims;
    ReadOnlyProperty(getInventors) QList<Key> inventors;

    // Getters
    virtual Key getId(void) override { return mDat.PatentMatterId; }
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

  public slots:
    bool slotAddInventor(QString first, QString last);
    bool slotAddInventors(QList<PersonData> inventors);
    bool slotAddExaminer(String first, String last);
    bool slotAddSupervisoryExaminer(String first, String last);

    bool slotUpdate(PatentMatterData dat);
    virtual bool slotSetId(Key newval) override;
    virtual bool slotCommit(void) override;

    bool slotSetFilingDate(Date newval);
    bool slotSetApplicationSerialNumber(String newval);
    bool slotSetConfirmationNumber(String newval);
    bool slotSetArtUnit(String newval);
    bool slotSetPatentNumber(String newval);
    bool slotSetfkExaminer(Key newval);
    bool slotSetfkFirstInventor(Key newval);
    bool slotSetfkSupervisoryExaminer(Key newval);
    bool slotSetfkApplicant(Key newval);
    bool slotSetBarDate(Date newval);
    bool slotSetCriticalDate(Date newval);
    bool slotSetfkSpecification(Key newval);
    bool slotSetfkDrawings(Key newval);
    bool slotSetfkAsFiledClaims(Key newval);

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
    const QString patentMatterTableName = "patentMatter";
};

} // namespace Nutmeg

#endif // NUTMEG_PATENTMATTER_H
