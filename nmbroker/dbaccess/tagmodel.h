#ifndef NUTMEG_TAGMODEL_H
#define NUTMEG_TAGMODEL_H

#include <QObject>
#include "tablemodel.h"

namespace Nutmeg {

class tagModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit tagModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_TAGMODEL_H
