#ifndef NUTMEG_TASKTYPECOMBO_H
#define NUTMEG_TASKTYPECOMBO_H

#include "combobox.h"
#include "dbaccess/tablemodel.h"
#include "dbaccess/tasktypemodel.h"
#include "dbaccess/viewcopyrightfilingtypesmodel.h"
#include "dbaccess/viewcopyrighttasktypesmodel.h"
#include "dbaccess/viewfilingtasktypesmodel.h"
#include "dbaccess/viewpatentfilingtypesmodel.h"
#include "dbaccess/viewpatenttasktypesmodel.h"
#include "dbaccess/viewresponsetasktypesmodel.h"
#include "dbaccess/viewtrademarkfilingtypesmodel.h"
#include "dbaccess/viewtrademarktasktypesmodel.h"
#include "objects/task.h"
#include <QObject>
#include <QWidget>

namespace Nutmeg
{

typedef enum
{
    PatentFiling,
    PatentResponse,
    TrademarkFiling,
    TrademarkResponse,
    CopyrightFiling,
    CopyrightResponse,
    Litigation,
    Other
} TaskClass;

class TaskTypeCombo : public Nutmeg::ComboBox
{
    Q_OBJECT
  public:
    TaskTypeCombo(std::shared_ptr<Task> task, QWidget *parent = nullptr);

  protected:
    std::shared_ptr<Task> mTask;

  protected slots:
};

} // namespace Nutmeg

#endif // NUTMEG_TASKTYPECOMBO_H
