#ifndef NUTMEG_VIEWCONTRACTINGFIRMSMODEL_H
#define NUTMEG_VIEWCONTRACTINGFIRMSMODEL_H

#include "tablemodel.h"
#include <QObject>

namespace Nutmeg
{

class viewContractingFirmsModel : public Nutmeg::TableModel
{
    Q_OBJECT
  public:
    explicit viewContractingFirmsModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWCONTRACTINGFIRMSMODEL_H
