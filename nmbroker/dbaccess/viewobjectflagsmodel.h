#ifndef NUTMEG_VIEWOBJECTFLAGSMODEL_H
#define NUTMEG_VIEWOBJECTFLAGSMODEL_H

#include <QObject>
#include "tablemodel.h"

namespace Nutmeg {

class viewObjectFlagsModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewObjectFlagsModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWOBJECTFLAGSMODEL_H
