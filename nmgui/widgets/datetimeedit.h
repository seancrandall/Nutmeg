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

    Property(getDate, setDate) QDate date;
    Property(getTime, setTime) QTime time;
    Property(getDateTime, setDateTime) QDateTime dateTime;

    QDate getDate(void) {return QDateTimeEdit::date();}
    void setDate(QDate newdate) {QDateTimeEdit::setDate(newdate);}
    QTime getTime(void){return QDateTimeEdit::time();}
    void setTime(QTime newtime) {QDateTimeEdit::setTime(newtime);}
    QDateTime getDateTime(void) {return QDateTimeEdit::dateTime();}
    void setDateTime(QDateTime newdatetime) {QDateTimeEdit::setDateTime(newdatetime);}

private:
    QDateTime mDateTime;

    void Initialize(void);
};

} // namespace Nutmeg

#endif // DATETIMEEDIT_H
