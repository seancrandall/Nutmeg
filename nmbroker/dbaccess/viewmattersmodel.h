#ifndef NUTMEG_VIEWMATTERSMODEL_H
#define NUTMEG_VIEWMATTERSMODEL_H

#include "tablemodel.h"
#include <QObject>

namespace Nutmeg
{

class viewMattersModel : public Nutmeg::TableModel
{
    Q_OBJECT
  public:
    explicit viewMattersModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWMATTERSMODEL_H
