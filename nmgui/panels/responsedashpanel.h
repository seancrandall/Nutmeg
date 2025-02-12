#ifndef NUTMEG_RESPONSEDASHPANEL_H
#define NUTMEG_RESPONSEDASHPANEL_H

#include <QHBoxLayout>
#include <QObject>
#include <QSqlDatabase>
#include <QWidget>

#include "nutmeg.h"
#include "frame.h"
#include "objects/deadline.h"
#include "objects/matter.h"
#include "objects/response.h"
#include "objects/task.h"

#include "panels/flagspanel.h"
#include "panels/entitiespanel.h"
#include "panels/responsetaskpanel.h"
#include "panels/deadlinespanel.h"

#include "widgets/combobox.h"
#include "widgets/donebutton.h"
#include "widgets/responsetypelabel.h"

namespace Nutmeg
{

class ResponseDashPanel : public Frame
{
    Q_OBJECT
  public:
    explicit ResponseDashPanel(Key responseId, QWidget *parent = nullptr);

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
    std::shared_ptr<Deadline> mDeadline;
    std::shared_ptr<Response> mResponse;
    std::shared_ptr<Matter> mMatter;

    DoneButton *doneButton;
    // Need a flags panel too

  private:
    void ConnectSignalsAndSlots(void);
    void LoadData(void);
};

} // namespace Nutmeg

#endif // NUTMEG_RESPONSEDASHPANEL_H
