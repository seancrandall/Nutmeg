#ifndef NUTMEG_DATEEDIT_H
#define NUTMEG_DATEEDIT_H

#include <QCalendarWidget>
#include <QDate>
#include <QDateEdit>
#include <QEvent>
#include <QObject>
#include <QWidget>

#include "property.h"

namespace Nutmeg
{

class DateEdit : public QDateEdit
{
    Q_OBJECT
  public:
    DateEdit(QDate date = QDate(QDate::currentDate()), QWidget *parent = nullptr);
    DateEdit(QWidget *parent = nullptr);

    Property(getDate, setDate) QDate date;

    QDate getDate(void) { return QDateEdit::date(); }

    void setDate(QDate newdate) { QDateEdit::setDate(newdate); }

  protected:
    bool eventFilter(QObject *object, QEvent *event);

  private:
    void InitializeDateEdit(void);
};

} // namespace Nutmeg

#endif // NUTMEG_DATEEDIT_H
