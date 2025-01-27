#ifndef DATETIMEEDIT_H
#define DATETIMEEDIT_H

#include <QDateTimeEdit>
#include <QObject>
#include <QWidget>
#include <QDate>

#include "nutmeg.h"
#include "property.h"

namespace Nutmeg {

class DateTimeEdit : public QDateTimeEdit
{
    Q_OBJECT
public:
    DateTimeEdit(QWidget *parent = nullptr);
    DateTimeEdit(QDateTime iniTime, QWidget *parent = nullptr);

    Property(date, setDate) QDate date;
    Property(time, setTime) QTime time;
    Property(dateTime, setDateTime) QDateTime dateTime;

private:
    QDateTime mDateTime;

    void Initialize(void);
};

} // namespace Nutmeg

#endif // DATETIMEEDIT_H
