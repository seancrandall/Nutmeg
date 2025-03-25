#ifndef SPINBOX_H
#define SPINBOX_H

#include <QObject>
#include <QSpinBox>
#include <QWidget>

namespace Nutmeg {

class SpinBox : public QSpinBox
{
    Q_OBJECT
public:
    SpinBox(QWidget *parent = nullptr);
};

} // namespace Nutmeg

#endif // SPINBOX_H
