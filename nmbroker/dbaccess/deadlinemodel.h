#ifndef NUTMEG_DEADLINEMODEL_H
#define NUTMEG_DEADLINEMODEL_H

#include <QObject>
#include "tablemodel.h"

namespace Nutmeg {

class deadlineModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit deadlineModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_DEADLINEMODEL_H
