#include "dateedit.h"

namespace Nutmeg
{

DateEdit::DateEdit(QDate date, QWidget *parent) : QDateEdit(date, parent) { InitializeDateEdit(); }

DateEdit::DateEdit(QWidget *parent) : QDateEdit(parent)
{
    // setDate(QDate::fromString("0001-01-01"));
    InitializeDateEdit();
}

bool DateEdit::eventFilter(QObject *object, QEvent *event)
{
    // Ignore hover events so we don't accidentally scroll combo boxes when the
    // mouse passes over them
    if (event->type() == QEvent::HoverEnter || event->type() == QEvent::HoverMove || event->type() == QEvent::Wheel)
    {
        return true;
    }
    return DateEdit::eventFilter(object, event);
}

void DateEdit::InitializeDateEdit()
{
    // setMinimumDate( QDate::fromString("0000-00-00") );
    setCalendarPopup(true);
    setDisplayFormat("yyyy-MM-dd");
    setMaximumWidth(120);
}

} // namespace Nutmeg
