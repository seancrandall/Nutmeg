#ifndef NUTMEG_VIEWJURISDICTIONSMODEL_H
#define NUTMEG_VIEWJURISDICTIONSMODEL_H

#include "tablemodel.h"
#include <QObject>

namespace Nutmeg
{

class viewJurisdictionsModel : public Nutmeg::TableModel
{
    Q_OBJECT
  public:
    explicit viewJurisdictionsModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWJURISDICTIONSMODEL_H
