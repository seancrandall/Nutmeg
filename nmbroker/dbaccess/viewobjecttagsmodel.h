#ifndef NUTMEG_VIEWOBJECTTAGSMODEL_H
#define NUTMEG_VIEWOBJECTTAGSMODEL_H

#include <QObject>
#include "tablemodel.h"

namespace Nutmeg {

class viewObjectTagsModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewObjectTagsModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWOBJECTTAGSMODEL_H
