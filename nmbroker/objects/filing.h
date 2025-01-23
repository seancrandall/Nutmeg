#ifndef NUTMEG_FILING_H
#define NUTMEG_FILING_H

#include "task.h"
#include <QObject>

namespace Nutmeg
{

class Filing : public Nutmeg::Task
{
    Q_OBJECT
  public:
    explicit Filing(QObject *parent = nullptr);
    explicit Filing(Key id, QObject *parent = nullptr);

    // Properties
    Property(getId, slotSetId) Key FilingId;
    Property(getfkFilingStatus, slotSetfkFilingStatus) Key fkFilingStatus;
    Property(getfkAsFiledDocument, slotSetfkAsFiledDocument) Key fkAsFiledDocument;

    // Getters
    virtual Key getId(void) override { return mDat.FilingId; }
    Key getfkFilingStatus(void) { return mDat.fkFilingStatus; }
    Key getfkAsFiledDocument(void) { return mDat.fkAsFiledDocument; }

  public slots:

    bool slotUpdate(FilingData dat);
    virtual bool slotSetId(Key newval) override;
    virtual bool slotCommit(void) override;

    bool setfkFilingStatus(Key newval);
    bool setfkAsFiledDocument(Key newval);

    void holdfkFilingStatus(Key newval){mDat.fkFilingStatus = newval; dirty["fkFilingStatus"] = true;}
    void holdfkFiledDocument(Key newval){mDat.fkAsFiledDocument = newval; dirty["fkAsfiledDocument"] = true;};


  protected:
    FilingData mDat;
    bool InitializeFiling(Key id);
    const QString filingTableName = "filing";
};

} // namespace Nutmeg

#endif // NUTMEG_FILING_H
