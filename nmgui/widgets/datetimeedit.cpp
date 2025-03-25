#include "datetimeedit.h"

namespace Nutmeg {

DateTimeEdit::DateTimeEdit(QWidget *parent)
    : QDateTimeEdit(parent)
{
    Initialize();
    mDateTime = QDateTime::currentDateTime();
}

DateTimeEdit::DateTimeEdit(QDateTime iniTime, QWidget *parent)
    : QDateTimeEdit(parent)
    , mDateTime(iniTime)
{
    Initialize();
    setDateTime(iniTime);
}

void DateTimeEdit::Initialize()
{
    setMinimumDate(QDate::fromString("1999-01-01", Qt::ISODate));

    setDisplayFormat("yyyy-MM-dd hh:mm");
    //Todo: Option for 24-hour clock or 12-hour clock

    setCalendarPopup(true);
}

} // namespace Nutmeg
