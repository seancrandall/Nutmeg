#ifndef NUTMEG_WORKATTORNEYSEARCHBOX_H
#define NUTMEG_WORKATTORNEYSEARCHBOX_H

#include "personsearchbox.h"
#include <QObject>
#include <QWidget>

namespace Nutmeg
{

class WorkAttorneySearchBox : public Nutmeg::PersonSearchBox
{
    Q_OBJECT
  public:
    WorkAttorneySearchBox(QWidget *parent = nullptr);
    WorkAttorneySearchBox(Key initkey, QWidget *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_WORKATTORNEYSEARCHBOX_H
