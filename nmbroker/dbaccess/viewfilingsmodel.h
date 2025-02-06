#ifndef NUTMEG_VIEWFILINGSMODEL_H
#define NUTMEG_VIEWFILINGSMODEL_H

#include <QObject>
#include "tablemodel.h"

namespace Nutmeg {

class viewFilingsModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewFilingsModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWFILINGSMODEL_H
