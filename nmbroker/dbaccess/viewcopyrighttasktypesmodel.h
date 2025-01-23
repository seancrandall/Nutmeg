#ifndef NUTMEG_VIEWCOPYRIGHTTASKTYPESMODEL_H
#define NUTMEG_VIEWCOPYRIGHTTASKTYPESMODEL_H

#include "tablemodel.h"
#include <QObject>

namespace Nutmeg
{

class viewCopyrightTaskTypesModel : public Nutmeg::TableModel
{
    Q_OBJECT
  public:
    explicit viewCopyrightTaskTypesModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWCOPYRIGHTTASKTYPESMODEL_H
