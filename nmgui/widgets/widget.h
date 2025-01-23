#ifndef NUTMEG_WIDGET_H
#define NUTMEG_WIDGET_H

#include <QWidget>

namespace Nutmeg
{

class Widget : public QWidget
{
    Q_OBJECT
  public:
    explicit Widget(QWidget *parent = nullptr);

  signals:
};

} // namespace Nutmeg

#endif // NUTMEG_WIDGET_H
