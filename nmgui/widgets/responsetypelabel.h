#ifndef NUTMEG_RESPONSETYPELABEL_H
#define NUTMEG_RESPONSETYPELABEL_H

#include <QLabel>
#include <QObject>
#include <QWidget>

#include "objects/response.h"

namespace Nutmeg
{

class ResponseTypeLabel : public QLabel
{
    Q_OBJECT
  public:
    ResponseTypeLabel(std::shared_ptr<Response> response, QWidget *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_RESPONSETYPELABEL_H
