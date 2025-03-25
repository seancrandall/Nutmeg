#ifndef NUTMEG_GROUPBOX_H
#define NUTMEG_GROUPBOX_H

#include <QGroupBox>
#include <QObject>
#include <QWidget>

#include "property.h"

namespace Nutmeg
{

class GroupBox : public QGroupBox
{
    Q_OBJECT
  public:
    GroupBox(QWidget *parent = nullptr);
    GroupBox(QString boxText, QWidget *parent = nullptr);

    Property(setParent, parentWidget) QWidget *parent;
};

} // namespace Nutmeg

#endif // NUTMEG_GROUPBOX_H
