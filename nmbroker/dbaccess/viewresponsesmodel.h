#ifndef NUTMEG_VIEWRESPONSESMODEL_H
#define NUTMEG_VIEWRESPONSESMODEL_H

#include <QObject>
#include "tablemodel.h"

namespace Nutmeg {

class viewResponsesModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewResponsesModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWRESPONSESMODEL_H
