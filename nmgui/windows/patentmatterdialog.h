#ifndef NUTMEG_PATENTMATTERDIALOG_H
#define NUTMEG_PATENTMATTERDIALOG_H

#include "matterdialog.h"
#include "nutmeg.h"
#include "property.h"
#include <QObject>
#include <QWidget>

#include "objects/patentmatter.h"

#include "panels/patentmatterpanel.h"
#include "panels/matterpanel.h"


namespace Nutmeg
{

//class PatentMatterDialog : public Nutmeg::MatterDialog
class PatentMatterDialog : public Nutmeg::Dialog
{
    Q_OBJECT
public:
    PatentMatterDialog(Key pmkey, QWidget *parent = nullptr);
    PatentMatterDialog(PatentMatter* matterin, QWidget *parent = nullptr);

public slots:
    virtual void slotScatter(void) override;
    virtual void slotGather(void) override;
    virtual void slotHandleRejected(void) override;
    virtual void slotHandleAccepted(void) override;

protected:
    PatentMatter* pmatter;
    PatentMatterPanel *pmPanel;
    MatterPanel *mPanel;
    QVBoxLayout *vlayout;

    virtual void connectSignalsAndSlots(void) override;
    virtual void setupDisplay(void) override;

private:
};

} // namespace Nutmeg

#endif // NUTMEG_PATENTMATTERDIALOG_H
