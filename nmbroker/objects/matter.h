#ifndef NUTMEG_MATTER_H
#define NUTMEG_MATTER_H

#include "object.h"
#include <QObject>

namespace Nutmeg
{

class Matter : public Nutmeg::Object
{
    Q_OBJECT
  public:
    explicit Matter(QObject *parent = nullptr);
    explicit Matter(Key id, QObject *parent = nullptr);
    //explicit Matter(const QSqlRecord matterRecord, QObject *parent=nullptr);
    explicit Matter(QString docketNumber, QObject *parent = nullptr);

    Property(getId, slotSetId) Key MatterId;
    Property(getfkParent, slotSetfkParent) Key fkParent;
    Property(getAttorneyDocketNumber, slotSetAttorneyDocketNumber) QString AttorneyDocketNumber;
    Property(getClientDocketNumber, slotSetClientDocketNumber) QString ClientDocketNumber;
    Property(getTitle, slotSetTitle) QString Title;
    Property(getfkClient, slotSetfkClient) Key fkClient;
    Property(getfkAssigningFirm, slotSetfkAssigningFirm) Key fkAssigningFirm;
    Property(getfkDefaultWorkAttorney, slotSetfkDefaultWorkAttorney) Key fkDefaultWorkAttorney;
    Property(getfkDefaultParalegal, slotSetfkDefaultParalegal) Key fkDefaultParalegal;
    Property(getfkKeyDocument, slotSetfkKeyDocument) Key fkKeyDocument;
    Property(getfkMatterJurisdiction, slotSetfkMatterJurisdiction) Key fkMatterJurisdiction;
    Property(getOldMatterId, slotSetOldMatterId) Key OldMatterId;

    // Getters
    virtual Key getId(void) override { return mDat.MatterId; }

    Key getfkParent(void);
    QString getAttorneyDocketNumber(void) { return mDat.AttorneyDocketNumber; }
    QString getClientDocketNumber(void) { return mDat.ClientDocketNumber; }
    QString getTitle(void) { return mDat.Title; }
    Key getfkClient(void) { return mDat.fkClient; }
    Key getfkAssigningFirm(void) { return mDat.fkAssigningFirm; }
    Key getfkDefaultWorkAttorney(void) { return mDat.fkDefaultWorkAttorney; }
    Key getfkDefaultParalegal(void) { return mDat.fkDefaultParalegal; }
    Key getfkKeyDocument(void) { return mDat.fkKeyDocument; }
    Key getfkMatterJurisdiction(void) { return mDat.fkMatterJurisdiction; }
    Key getOldMatterId(void) { return mDat.OldMatterId; }

  public slots:
    virtual bool SetId(Key id) override;
    bool slotUpdate(MatterData dat);
    virtual bool Commit(void) override;

    bool slotSetfkParent(Key newfk);
    bool slotSetAttorneyDocketNumber(QString newnum);
    bool slotSetClientDocketNumber(QString newnum);
    bool slotSetTitle(QString newtitle);
    bool slotSetfkClient(Key newfk);
    bool slotSetfkAssigningFirm(Key newfk);
    bool slotSetfkDefaultWorkAttorney(Key newfk);
    bool slotSetfkDefaultParalegal(Key newfk);
    bool slotSetfkKeyDocument(Key newfk);
    bool slotSetfkMatterJurisdiction(Key newfk);
    bool slotSetOldId(Key newold); /// Seriously, why are you trying to do this? This value is
                                   /// only useful for imports from the old database

    void holdfkParent(Key newval) {mDat.fkParent = newval; dirty["fkParent"] = true;}
    void holdAttorneyDocketNumber(QString newval) {mDat.AttorneyDocketNumber = newval;dirty["AttorneyDocketNumber"] = true;}
    void holdClientDocketNumber(QString newval) {mDat.ClientDocketNumber = newval;dirty["ClientDocketNumber"] = true;}
    void holdTitle(QString newval) {mDat.Title = newval;dirty["Title"] = true;}
    void holdfkClient(Key newval) {mDat.fkClient = newval;dirty["fkClient"] = true;}
    void holdfkAssigningFirm(Key newval) {mDat.fkAssigningFirm = newval;dirty["fkAssigningFirm"] = true;}
    void holdfkDefaultWorkAttorney(Key newval) {mDat.fkDefaultWorkAttorney = newval;dirty["fkDefaultWorkAttorney"] = true;}
    void holdfkDefaultParalegal(Key newval) {mDat.fkDefaultParalegal = newval;dirty["fkDefaultParalegal"] = true;}
    void holdfkKeyDocument(Key newval) {mDat.fkKeyDocument = newval;dirty["fkKeyDocument"] = true;}
    void holdfkMatterJurisdiction(Key newval) {mDat.fkMatterJurisdiction = newval;dirty["fkMatterJurisdiction"] = true;}
    void holdOldId(Key newval) {mDat.OldMatterId = newval;dirty["OldMatterId"] = true;}

  protected:
    MatterData mDat;
    bool InitializeMatter(Key id);
    const QString matterTableName = "matter";
};

inline Key Matter::getfkParent() { return mDat.fkParent; }

} // namespace Nutmeg

#endif // NUTMEG_MATTER_H
