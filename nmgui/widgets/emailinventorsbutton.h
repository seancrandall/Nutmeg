#ifndef EMAILINVENTORSBUTTON_H
#define EMAILINVENTORSBUTTON_H

#include <QObject>
#include <QWidget>

#include "objects/filing.h"
#include "pushbutton.h"

namespace Nutmeg
{

class EmailInventorsButton : public Nutmeg::PushButton
{
    Q_OBJECT
  public:
    EmailInventorsButton(std::shared_ptr<Filing> fil, QWidget *parent = nullptr);

  public slots:
    void slotHandleClick(void);

  protected:
    std::shared_ptr<Filing> mFiling;
};

} // namespace Nutmeg

#endif // EMAILINVENTORSBUTTON_H
