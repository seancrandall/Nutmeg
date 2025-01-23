#ifndef NUTMEG_LABEL_H
#define NUTMEG_LABEL_H

#include <QLabel>
#include <QObject>
#include <QWidget>

#include "property.h"

namespace Nutmeg
{

class Label : public QLabel
{
    Q_OBJECT
  public:
    Label(QWidget *parent = nullptr);
    Label(const QString &text, QWidget *parent = nullptr);

    Property(text, setText) QString text;
};

} // namespace Nutmeg

#endif // NUTMEG_LABEL_H
