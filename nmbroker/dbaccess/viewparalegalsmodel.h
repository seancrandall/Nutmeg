#ifndef NUTMEG_VIEWPARALEGALSMODEL_H
#define NUTMEG_VIEWPARALEGALSMODEL_H

#include "tablemodel.h"
#include <QObject>

namespace Nutmeg
{

class viewParalegalsModel : public Nutmeg::TableModel
{
    Q_OBJECT
  public:
    explicit viewParalegalsModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWPARALEGALSMODEL_H
