#ifndef DOUBLESPINBOX_H
#define DOUBLESPINBOX_H

#include <QDoubleSpinBox>
#include <QObject>
#include <QWidget>

namespace Nutmeg {

class DoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
public:
    DoubleSpinBox(QWidget *parent = nullptr);
};

} // namespace Nutmeg

#endif // DOUBLESPINBOX_H
