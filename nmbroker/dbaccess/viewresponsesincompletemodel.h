#ifndef NUTMEG_VIEWRESPONSESINCOMPLETEMODEL_H
#define NUTMEG_VIEWRESPONSESINCOMPLETEMODEL_H

#include "tablemodel.h"
#include <QObject>

namespace Nutmeg
{

class viewResponsesIncompleteModel : public Nutmeg::TableModel
{
    Q_OBJECT
  public:
    explicit viewResponsesIncompleteModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWRESPONSESINCOMPLETEMODEL_H
