#ifndef NUTMEG_VIEWPEOPLEMODEL_H
#define NUTMEG_VIEWPEOPLEMODEL_H

#include "tablemodel.h"
#include <QObject>

namespace Nutmeg
{

class viewPeopleModel : public Nutmeg::TableModel
{
    Q_OBJECT
  public:
    explicit viewPeopleModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWPEOPLEMODEL_H
