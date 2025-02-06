#ifndef NUTMEG_FLAGCLASSMODEL_H
#define NUTMEG_FLAGCLASSMODEL_H

#include <QObject>
#include "tablemodel.h"

namespace Nutmeg {

class flagClassModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit flagClassModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_FLAGCLASSMODEL_H
