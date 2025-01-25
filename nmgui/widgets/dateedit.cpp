#include "dateedit.h"

namespace Nutmeg
{

DateEdit::DateEdit(QDate dateinit, QWidget *parent)
    : QDateEdit(dateinit, parent)
{
    date = dateinit;
    InitializeDateEdit();
}

DateEdit::DateEdit(QWidget *parent)
    : QDateEdit(parent)
{
    date = QDate::fromString("1999-01-01", Qt::ISODate);
    InitializeDateEdit();
}

void DateEdit::setDate(QDate newdate)
{
    QDateEdit::setDate(newdate);
}

void DateEdit::setColor(QColor newColor)
{
    mColor = newColor;
    update(); //Trigger repaint event
    //setStyleSheet("background-color: darkred");
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

void DateEdit::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    // Use the current color to set the background
    p.fillRect(rect(), QBrush(mColor));

    // Call the parent's paint event to draw the date edit contents
    QDateEdit::paintEvent(event);
}

void DateEdit::InitializeDateEdit()
{
    setMinimumDate(QDate::fromString("1970-01-01", Qt::ISODate));
    setCalendarPopup(true);
    setDisplayFormat("yyyy-MM-dd");
    setMaximumWidth(120);

    //If non-date, change color
    // QDate nonDate = QDate::fromString("2000-01-01", Qt::ISODate);
    // if (date <= nonDate)
    // {
    //     //setStyleSheet("background-color: red");
    //     //setColor(Qt::red);
    // }
}

} // namespace Nutmeg
