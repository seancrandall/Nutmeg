#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H

#include <QObject>
#include <QTabWidget>
#include <QWidget>

#include "dialog.h"
#include "settings.h"

#include "panels/optionsdatabasepanel.h"
#include "panels/optionsdisplaypanel.h"
#include "panels/optionsdocumentpanel.h"

namespace Nutmeg
{

class OptionsWindow : public Nutmeg::Dialog
{
    Q_OBJECT
  public:
    OptionsWindow(Settings *settings, QWidget *parent = nullptr);

  protected:
    void slotScatter(void) override;
    void slotGather(void) override;

    void slotHandleRejected(void) override;
    void slotHandleAccepted(void) override;

    void setupDisplay(void) override;
    void connectSignalsAndSlots(void) override;

  private:
    OptionsDatabasePanel *optionsDatabasePanel;
    OptionsDisplayPanel *optionsDisplayPanel;
    OptionsDocumentPanel *optionsDocumentsPanel;

    Settings *mSettings;
    QTabWidget *tabber;
};

} // namespace Nutmeg

#endif // OPTIONSWINDOW_H
