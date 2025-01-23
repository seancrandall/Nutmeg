#ifndef NUTMEG_TRADEMARKMATTER_H
#define NUTMEG_TRADEMARKMATTER_H

#include "matter.h"
#include <QObject>

namespace Nutmeg
{

class TrademarkMatter : public Nutmeg::Matter
{
    Q_OBJECT
  public:
    explicit TrademarkMatter(QObject *parent = nullptr);
    explicit TrademarkMatter(Key id, QObject *parent = nullptr);
    explicit TrademarkMatter(QString docketNumber, QObject *parent = nullptr);

    // Properties
    Property(getId, slotSetId) Key TrademarkMatterId;
    Property(getFirstUseInCommerce, slotSetFirstUseInCommerce) Date FirstUseInCommerce;
    Property(getInternationalClass, slotSetInternationalClass) int InternationalClass;
    Property(getfkStatus, slotSetfkStatus) Key fkStatus;
    Property(getSerialNumber, slotSetSerialNumber) String SerialNumber;
    Property(getRegistrationNumber, slotSetRegistrationNumber) String RegistrationNumber;
    Property(getPublicationDate, slotSetPublicationDate) Date PublicationDate;
    Property(getWindowOpens, slotSetWindowOpens) Date WindowOpens;
    Property(getNofeeWindowCloses, slotSetNofeeWindowCloses) Date NofeeWindowCloses;
    Property(getFinalWindowCloses, slotSetFinalWindowCloses) Date FinalWindowCloses;
    Property(getfkTrademarkExaminer, slotSetfkTrademarkExaminer) Key fkTrademarkExaminer;
    Property(getfkFilingBasis, slotSetfkFilingBasis) Key fkFilingBasis;
    Property(getfkTrademarkJurisdiction, slotSetfkTrademarkJurisdiction) Key fkTrademarkJurisdiction;
    Property(getfkSpecimen, slotSetfkSpecimen) Key fkSpecimen;
    Property(getfkEvidenceOfUse, slotSetfkEvidenceOfUse) Key fkEvidenceOfUse;
    Property(getMark, slotSetMark) String Mark;
    Property(getGoodsServices, slotSetGoodsServices) String GoodsServices;

    // Getters
    virtual Key getId(void) override { return mDat.TrademarkMatterId; }
    Date getFirstUseInCommerce(void) { return mDat.FirstUseInCommerce; }
    int getInternationalClass(void) { return mDat.InternationalClass; }
    Key getfkStatus(void) { return mDat.fkStatus; }
    String getSerialNumber(void) { return mDat.SerialNumber; }
    String getRegistrationNumber(void) { return mDat.RegistrationNumber; }
    Date getPublicationDate(void) { return mDat.PublicationDate; }
    Date getWindowOpens(void) { return mDat.WindowOpens; }
    Date getNofeeWindowCloses(void) { return mDat.NofeeWindowCloses; }
    Date getFinalWindowCloses(void) { return mDat.FinalWindowCloses; }
    Key getfkTrademarkExaminer(void) { return mDat.fkTrademarkExaminer; }
    Key getfkFilingBasis(void) { return mDat.fkFilingBasis; }
    Key getfkTrademarkJurisdiction(void) { return mDat.fkTrademarkJurisdiction; }
    Key getfkSpecimen(void) { return mDat.fkSpecimen; }
    Key getfkEvidenceOfUse(void) { return mDat.fkEvidenceOfUse; }
    String getMark(void) { return mDat.Mark; }
    String getGoodsServices(void) { return mDat.GoodsServices; }

  public slots:
    bool slotUpdate(TrademarkMatterData dat);
    virtual bool slotSetId(Key newval) override;;
    virtual bool slotCommit(void) override;

    bool slotSetFirstUseInCommerce(Date newval);
    bool slotSetInternationalClass(int newval);
    bool slotSetfkStatus(Key newval);
    bool slotSetSerialNumber(String newval);
    bool slotSetRegistrationNumber(String newval);
    bool slotSetPublicationDate(Date newval);
    bool slotSetWindowOpens(Date newval);
    bool slotSetNofeeWindowCloses(Date newval);
    bool slotSetFinalWindowCloses(Date newval);
    bool slotSetfkTrademarkExaminer(Key newval);
    bool slotSetfkFilingBasis(Key newval);
    bool slotSetfkTrademarkJurisdiction(Key newval);
    bool slotSetfkSpecimen(Key newval);
    bool slotSetfkEvidenceOfUse(Key newval);
    bool slotSetMark(String newval);
    bool slotSetGoodsServices(String newval);

    void holdFirstUseInCommerce(Date newval) {mDat.FirstUseInCommerce = newval;}
    void holdInternationalClass(int newval){mDat.InternationalClass = newval;};
    void holdfkStatus(Key newval){mDat.fkStatus = newval;};
    void holdSerialNumber(String newval){mDat.SerialNumber = newval;};
    void holdRegistrationNumber(String newval){mDat.RegistrationNumber = newval;};
    void holdPublicationDate(Date newval){mDat.PublicationDate = newval;};
    void holdWindowOpens(Date newval){mDat.WindowOpens = newval;};
    void holdNoFeeWindowCloses(Date newval){mDat.NofeeWindowCloses = newval;};
    void holdFinalWindowCloses(Date newval){mDat.FinalWindowCloses = newval;};
    void holdfkTrademarkExaminer(Key newval){mDat.fkTrademarkExaminer = newval;};
    void holdfkFilingBasis(Key newval){mDat.fkFilingBasis = newval;};
    void holdfkTrademarkJurisdiction(Key newval){mDat.fkTrademarkJurisdiction = newval;};
    void holdfkSpecimen(Key newval){mDat.fkSpecimen = newval;};
    void holdfkEvidenceOfUse(Key newval){mDat.fkEvidenceOfUse = newval;};
    void holdMark(String newval){mDat.Mark = newval;};
    void holdGoodsServices(String newval){mDat.GoodsServices = newval;};

  protected:
    bool InitializeTrademarkMatter(Key id);

    TrademarkMatterData mDat;
    const QString trademarkMatterTableName = "trademarkMatter";
};

} // namespace Nutmeg

#endif // NUTMEG_TRADEMARKMATTER_H
