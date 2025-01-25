#ifndef NUTMEG_DATEEDIT_H
#define NUTMEG_DATEEDIT_H

#include <QCalendarWidget>
#include <QDate>
#include <QDateEdit>
#include <QEvent>
#include <QObject>
#include <QWidget>
#include <QStyleOption>
#include <QPainter>

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
    Property(getColor, setColor) QColor color;

    QDate getDate(void) { return QDateEdit::date(); }
    QColor getColor(void) { return mColor;}

    void setDate(QDate newdate);
    void setColor(QColor newColor);

  protected:
    bool eventFilter(QObject *object, QEvent *event) override;

    //Change color
    void paintEvent(QPaintEvent *event) override;

  private:
    void InitializeDateEdit(void);

      QColor mColor;
};

} // namespace Nutmeg

#endif // NUTMEG_DATEEDIT_H
