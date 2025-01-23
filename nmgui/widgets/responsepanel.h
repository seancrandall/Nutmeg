#ifndef NUTMEG_RESPONSEPANEL_H
#define NUTMEG_RESPONSEPANEL_H

#include <QHBoxLayout>
#include <QObject>
#include <QSqlDatabase>
#include <QWidget>

#include "nutmeg.h"
#include "objects/deadline.h"
#include "objects/matter.h"
#include "objects/response.h"
#include "objects/task.h"

#include "widgets/combobox.h"
#include "widgets/deadlinespanel.h"
#include "widgets/donebutton.h"
#include "widgets/responsetaskpanel.h"
#include "widgets/responsetypelabel.h"
#include "widgets/entitiespanel.h"
#include "widgets/flagspanel.h"

namespace Nutmeg
{

class ResponsePanel : public QWidget
{
    Q_OBJECT
  public:
    explicit ResponsePanel(Key responseId, QWidget *parent = nullptr);

  public slots:
    void slotUpdateParalegal(Key newkey);
    void slotUpdateWorkAttorney(Key newkey);
    void slotUpdateClient(Key newkey);
    void slotUpdateCompletion(void);
    void slotRefreshView(void);

  signals:
    void signalAddNewResponse(void);
    void signalSomethingChanged(void);

  protected:
    // Deadline *deadline;
    std::unique_ptr<Deadline> deadline;
    Response response;
    Task task;
    Matter matter;

    DoneButton *doneButton;
    // Need a flags panel too

  private:
    void ConnectSignalsAndSlots(void);
    void LoadData(void);
};

} // namespace Nutmeg

#endif // NUTMEG_RESPONSEPANEL_H
