#ifndef NUTMEG_VIEWTASKSMODEL_H
#define NUTMEG_VIEWTASKSMODEL_H

#include <QObject>
#include "tablemodel.h"

namespace Nutmeg {

class viewTasksModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewTasksModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWTASKSMODEL_H
