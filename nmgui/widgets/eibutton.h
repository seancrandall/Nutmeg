#ifndef NUTMEG_EIBUTTON_H
#define NUTMEG_EIBUTTON_H

#include "objects/response.h"
#include "pushbutton.h"
#include <QObject>
#include <QWidget>

namespace Nutmeg
{

class EIButton : public Nutmeg::PushButton
{
    Q_OBJECT
  public:
    EIButton(std::shared_ptr<Response> resp, QWidget *parent = nullptr);

  public slots:
    void slotHandleClick(void);

  protected:
    std::shared_ptr<Response> mResponse;
};

} // namespace Nutmeg

#endif // NUTMEG_EIBUTTON_H
