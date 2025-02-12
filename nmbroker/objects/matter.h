#ifndef NUTMEG_MATTER_H
#define NUTMEG_MATTER_H

#include "object.h"

#define matterTableName "matter"
namespace Nutmeg
{

class Matter : public Nutmeg::Object
{
  public:
    explicit Matter();
    explicit Matter(Key id);
    explicit Matter(QString docketNumber);
    virtual ~Matter() = default;

    Property(getId, SetId) Key MatterId;
    Property(getfkParent, SetfkParent) Key fkParent;
    Property(getAttorneyDocketNumber, SetAttorneyDocketNumber) QString AttorneyDocketNumber;
    Property(getClientDocketNumber, SetClientDocketNumber) QString ClientDocketNumber;
    Property(getTitle, SetTitle) QString Title;
    Property(getfkClient, SetfkClient) Key fkClient;
    Property(getfkAssigningFirm, SetfkAssigningFirm) Key fkAssigningFirm;
    Property(getfkDefaultWorkAttorney, SetfkDefaultWorkAttorney) Key fkDefaultWorkAttorney;
    Property(getfkDefaultParalegal, SetfkDefaultParalegal) Key fkDefaultParalegal;
    Property(getfkKeyDocument, SetfkKeyDocument) Key fkKeyDocument;
    Property(getfkMatterJurisdiction, SetfkMatterJurisdiction) Key fkMatterJurisdiction;
    Property(getOldMatterId, SetOldMatterId) Key OldMatterId;

    // Getters
    virtual Key getId(void) const override { return mDat.MatterId; }

    Key getfkParent(void) const;
    QString getAttorneyDocketNumber(void) const { return mDat.AttorneyDocketNumber; }
    QString getClientDocketNumber(void) const { return mDat.ClientDocketNumber; }
    QString getTitle(void) const { return mDat.Title; }
    Key getfkClient(void) const { return mDat.fkClient; }
    Key getfkAssigningFirm(void) const { return mDat.fkAssigningFirm; }
    Key getfkDefaultWorkAttorney(void) const { return mDat.fkDefaultWorkAttorney; }
    Key getfkDefaultParalegal(void) const { return mDat.fkDefaultParalegal; }
    Key getfkKeyDocument(void) const { return mDat.fkKeyDocument; }
    Key getfkMatterJurisdiction(void) const { return mDat.fkMatterJurisdiction; }
    Key getOldMatterId(void) const { return mDat.OldMatterId; }

    virtual bool SetId(Key id) override;
    bool Update(MatterData dat);
    virtual bool Commit(void) override;

    bool SetfkParent(Key newfk);
    bool SetAttorneyDocketNumber(QString newnum);
    bool SetClientDocketNumber(QString newnum);
    bool SetTitle(QString newtitle);
    bool SetfkClient(Key newfk);
    bool SetfkAssigningFirm(Key newfk);
    bool SetfkDefaultWorkAttorney(Key newfk);
    bool SetfkDefaultParalegal(Key newfk);
    bool SetfkKeyDocument(Key newfk);
    bool SetfkMatterJurisdiction(Key newfk);
    bool SetOldId(Key newold); /// Seriously, why are you trying to do this? This value is
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
private:
    static Matter* GetMatter(Key id);
};

} // namespace Nutmeg

#endif // NUTMEG_MATTER_H
