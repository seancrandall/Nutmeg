#ifndef NUTMEG_VIEWINVENTORSMODEL_H
#define NUTMEG_VIEWINVENTORSMODEL_H

#include "tablemodel.h"
#include <QObject>

namespace Nutmeg
{

class viewInventorsModel : public Nutmeg::TableModel
{
    Q_OBJECT
  public:
    explicit viewInventorsModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWINVENTORSMODEL_H
