#ifndef FILINGPANEL_H
#define FILINGPANEL_H

#include <QWidget>
#include <QHBoxLayout>

#include "nutmeg.h"
#include "objects/deadline.h"
#include "objects/matter.h"
#include "objects/response.h"
#include "objects/task.h"

#include "widgets/combobox.h"
#include "widgets/donebutton.h"
#include "widgets/filingtypelabel.h"
#include "panels/deadlinespanel.h"
#include "panels/filingtaskpanel.h"
#include "panels/entitiespanel.h"
#include "panels/flagspanel.h"

#include "dbaccess/viewclientsmodel.h"
#include "dbaccess/viewparalegalsmodel.h"
#include "dbaccess/viewworkattorneysmodel.h"

namespace Nutmeg
{

class FilingPanel : public QWidget
{
    Q_OBJECT
  public:
    explicit FilingPanel(Key id, QWidget *parent = nullptr);

  public slots:
    void slotUpdateCompletion(void);
    void slotRefreshView(void);

  signals:
    void signalAddNewFiling(void);
    void signalSomethingChanged(void);

  protected:
    std::shared_ptr<Deadline> deadline;
    std::shared_ptr<Filing> filing;
    std::shared_ptr<Matter> matter;

    DoneButton *doneButton;

  private:
    void ConnectSignalsAndSlots(void);
    void LoadData(void);
};

} // namespace Nutmeg

#endif // FILINGPANEL_H
