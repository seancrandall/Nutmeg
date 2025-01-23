#ifndef NUTMEG_VIEWTRADEMARKTASKTYPESMODEL_H
#define NUTMEG_VIEWTRADEMARKTASKTYPESMODEL_H

#include "tablemodel.h"
#include <QObject>

namespace Nutmeg
{

class viewTrademarkTaskTypesModel : public Nutmeg::TableModel
{
    Q_OBJECT
  public:
    explicit viewTrademarkTaskTypesModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWTRADEMARKTASKTYPESMODEL_H
