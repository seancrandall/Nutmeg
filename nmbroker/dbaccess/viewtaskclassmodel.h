#ifndef NUTMEG_VIEWTASKCLASSMODEL_H
#define NUTMEG_VIEWTASKCLASSMODEL_H

#include "tablemodel.h"
#include <QObject>

namespace Nutmeg
{

class viewTaskClassModel : public Nutmeg::TableModel
{
    Q_OBJECT
  public:
    explicit viewTaskClassModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWTASKCLASSMODEL_H
