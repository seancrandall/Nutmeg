#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <QCheckBox>
#include <QObject>
#include <QWidget>

namespace Nutmeg {

class CheckBox : public QCheckBox
{
    Q_OBJECT
public:
    CheckBox(QWidget *parent = nullptr);
};

} // namespace Nutmeg

#endif // CHECKBOX_H
