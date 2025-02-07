#ifndef NUTMEG_VIEWTRADEMARKMATTERSMODEL_H
#define NUTMEG_VIEWTRADEMARKMATTERSMODEL_H

#include <QObject>
#include "tablemodel.h"

namespace Nutmeg {

class viewTrademarkMattersModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewTrademarkMattersModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWTRADEMARKMATTERSMODEL_H
