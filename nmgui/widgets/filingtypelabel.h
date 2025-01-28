#ifndef FILINGTYPELABEL_H
#define FILINGTYPELABEL_H

#include <QLabel>
#include <QObject>
#include <QWidget>

#include "objects/filing.h"

namespace Nutmeg
{

class FilingTypeLabel : public QLabel
{
    Q_OBJECT
  public:
    FilingTypeLabel(std::shared_ptr<Filing> filing, QWidget *parent = nullptr);
};

} // namespace Nutmeg

#endif // FILINGTYPELABEL_H
