#ifndef NUTMEG_MATTERDIALOG_H
#define NUTMEG_MATTERDIALOG_H

#include <QObject>
#include <QWidget>

#include "windows/dialog.h"
#include "widgets/matterpanel.h"

namespace Nutmeg
{

class MatterDialog : public Nutmeg::Dialog
{
    Q_OBJECT
  public:
    MatterDialog(Key matterid, QWidget *parent = nullptr);


  protected:
    void setupDisplay(void) override;
    void connectSignalsAndSlots(void) override;
    void slotGather(void) override;
    void slotScatter(void) override;

    void layoutWorkspace(void);

    Matter matter;
    MatterPanel *panel;
};

} // namespace Nutmeg

#endif // NUTMEG_MATTERDIALOG_H
