#ifndef NUTMEG_VIEWRESPONSETASKTYPESMODEL_H
#define NUTMEG_VIEWRESPONSETASKTYPESMODEL_H

#include "tablemodel.h"
#include <QObject>

namespace Nutmeg
{

class viewResponseTaskTypesModel : public Nutmeg::TableModel
{
    Q_OBJECT
  public:
    explicit viewResponseTaskTypesModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWRESPONSETASKTYPESMODEL_H