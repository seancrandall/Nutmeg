#ifndef NUTMEG_VIEWPATENTTASKTYPESMODEL_H
#define NUTMEG_VIEWPATENTTASKTYPESMODEL_H

#include "tablemodel.h"
#include <QObject>

namespace Nutmeg
{

class viewPatentTaskTypesModel : public Nutmeg::TableModel
{
    Q_OBJECT
  public:
    explicit viewPatentTaskTypesModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWPATENTTASKTYPESMODEL_H
