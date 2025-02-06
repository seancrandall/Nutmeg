#ifndef NUTMEG_TASKMODEL_H
#define NUTMEG_TASKMODEL_H

#include <QObject>
#include "tablemodel.h"

namespace Nutmeg {

class taskModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit taskModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_TASKMODEL_H
