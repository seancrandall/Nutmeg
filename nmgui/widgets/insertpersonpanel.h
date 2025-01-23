#ifndef NUTMEG_INSERTPERSONPANEL_H
#define NUTMEG_INSERTPERSONPANEL_H

#include <QObject>
#include <QWidget>

#include "dbaccess/nutdb.h"
#include "property.h"
#include "widgets/labeledwidgetleft.h"
#include "widgets/lineedit.h"

namespace Nutmeg
{

class InsertPersonPanel : public QWidget
{
    Q_OBJECT
  public:
    explicit InsertPersonPanel(QWidget *parent = nullptr);

    Property(getFirstName, setFirstName) QString FirstName;
    Property(getLastName, setLastName) QString LastName;
    ReadOnlyProperty(getLayout) QVBoxLayout *layout;

    QString getFirstName(void) { return cFirstName->text; }

    QString getLastName(void) { return cLastName->text; }

    QVBoxLayout *getLayout(void) { return box; }

    void setFirstName(QString newname) { cLastName->text = newname; }

    void setLastName(QString newname) { cFirstName->text = newname; }

  protected:
    LineEdit *cFirstName = new LineEdit();
    LineEdit *cLastName = new LineEdit();

    LabeledWidgetLeft *lFirstName = new LabeledWidgetLeft("First Name", cFirstName);
    LabeledWidgetLeft *lLastName = new LabeledWidgetLeft("Last Name", cLastName);

    QVBoxLayout *box = new QVBoxLayout();

  signals:
};

} // namespace Nutmeg

#endif // NUTMEG_INSERTPERSONPANEL_H
