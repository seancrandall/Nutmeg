#ifndef NUTMEG_INSERTENTERPRISEPANEL_H
#define NUTMEG_INSERTENTERPRISEPANEL_H

#include <QObject>
#include <QWidget>

#include "property.h"
#include "widgets/labeledwidgetleft.h"
#include "widgets/lineedit.h"

namespace Nutmeg
{

class InsertEnterprisePanel : public QWidget
{
    Q_OBJECT
  public:
    explicit InsertEnterprisePanel(QWidget *parent = nullptr);

    // Properties
    Property(getEnterpriseName, setEnterpriseName) QString EnterpriseName;
    ReadOnlyProperty(getLayout) QHBoxLayout *layout;

    QString getEnterpriseName(void) { return cEnterpriseName->text; }

    void setEnterpriseName(QString newname) { cEnterpriseName->text = newname; }

    QHBoxLayout *getLayout(void) { return lEnterpriseName; }

  signals:

  protected:
    LineEdit *cEnterpriseName = new LineEdit();
    LabeledWidgetLeft *lEnterpriseName = new LabeledWidgetLeft("Name", cEnterpriseName);
};

} // namespace Nutmeg

#endif // NUTMEG_INSERTENTERPRISEPANEL_H
