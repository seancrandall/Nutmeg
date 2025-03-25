#ifndef TIMEEDIT_H
#define TIMEEDIT_H

#include <QObject>
#include <QTimeEdit>
#include <QWidget>

#include "nutmeg.h"
#include "property.h"

namespace Nutmeg {

class TimeEdit : public QTimeEdit
{
    Q_OBJECT
public:
    TimeEdit(QWidget *parent = nullptr);
    TimeEdit(const QTime& newtime, QWidget *parent=nullptr);

    Property(getTime, setTime) QTime time;

    const QTime getTime(void) const {return QTimeEdit::time();}
    void setTime(const QTime& newtime) {QTimeEdit::setTime(newtime);}
};

} // namespace Nutmeg

#endif // TIMEEDIT_H
