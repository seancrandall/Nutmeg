#ifndef NUTMEG_VIEWWORKATTORNEYSMODEL_H
#define NUTMEG_VIEWWORKATTORNEYSMODEL_H

#include "tablemodel.h"
#include <QObject>

namespace Nutmeg
{

class viewWorkAttorneysModel : public Nutmeg::TableModel
{
    Q_OBJECT
  public:
    explicit viewWorkAttorneysModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWWORKATTORNEYSMODEL_H
