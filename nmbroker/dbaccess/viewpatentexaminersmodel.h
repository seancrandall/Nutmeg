#ifndef NUTMEG_VIEWPATENTEXAMINERSMODEL_H
#define NUTMEG_VIEWPATENTEXAMINERSMODEL_H

#include "tablemodel.h"
#include <QObject>

namespace Nutmeg
{

class viewPatentExaminersModel : public Nutmeg::TableModel
{
    Q_OBJECT
  public:
    explicit viewPatentExaminersModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWPATENTEXAMINERSMODEL_H
