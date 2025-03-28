#ifndef NUTMEG_DOCKETNUMBERBUTTON_H
#define NUTMEG_DOCKETNUMBERBUTTON_H

#include <QObject>
#include <QWidget>

#include "objects/matter.h"
#include "pushbutton.h"

namespace Nutmeg
{

class DocketNumberButton : public Nutmeg::PushButton
{
    Q_OBJECT
  public:
    DocketNumberButton(std::shared_ptr<Matter> matter, QWidget *parent);

  public slots:
    void clicked();

  protected:
    std::shared_ptr<Matter> mMatter;
};

} // namespace Nutmeg

#endif // NUTMEG_DOCKETNUMBERBUTTON_H
