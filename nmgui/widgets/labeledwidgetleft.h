#ifndef NUTMEG_LABELEDWIDGETLEFT_H
#define NUTMEG_LABELEDWIDGETLEFT_H

#include <QHBoxLayout>
#include <QLabel>
#include <QObject>
#include <QWidget>

#include "nutmeg.h"
#include "property.h"

namespace Nutmeg
{

class LabeledWidgetLeft : public QHBoxLayout
{
    Q_OBJECT
  public:
    LabeledWidgetLeft(const QString label, QWidget *widget, QWidget *parent = nullptr);

    // Properties
    Property(getLabelText, setLabelText) QString LabelText;

    QString getLabelText(void) { return mLabel->text(); }

    void setLabelText(const QString &newtext) { mLabel->setText(newtext); }

  private:
    // QWidget *mWidget;
    QLabel *mLabel;
};

} // namespace Nutmeg

#endif // NUTMEG_LABELEDWIDGETLEFT_H
