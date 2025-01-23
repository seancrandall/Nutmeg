#ifndef NUTMEG_INFORMATIONBOX_H
#define NUTMEG_INFORMATIONBOX_H

#include "messagebox.h"
#include <QObject>
#include <QWidget>

namespace Nutmeg
{

class InformationBox : public Nutmeg::MessageBox
{
    Q_OBJECT
  public:
    InformationBox(QWidget *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_INFORMATIONBOX_H
