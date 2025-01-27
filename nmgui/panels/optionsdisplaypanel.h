#ifndef OPTIONSDISPLAYPANEL_H
#define OPTIONSDISPLAYPANEL_H

#include <QFont>
#include <QFontComboBox>
#include <QObject>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

#include "property.h"
#include "widgets/labeledwidgetleft.h"

namespace Nutmeg
{

class OptionsDisplayPanel : public QWidget
{
    Q_OBJECT
  public:
    explicit OptionsDisplayPanel(QWidget *parent = nullptr);

    Property(getFontFamily, setFontFamily) QString fontFamily;
    Property(getFontSize, setFontSize) uint fontSize;

    const QString getFontFamily(void) const { return eFontFamily->font().family(); };

    uint getFontSize(void) const { return eFontSize->value(); };

    void setFontFamily(const QString newfam);

    void setFontSize(uint newsize) { eFontSize->setValue(newsize); };

  private:
    QFontComboBox *eFontFamily;
    QSpinBox *eFontSize;

    LabeledWidgetLeft *wFontFamily;
    LabeledWidgetLeft *wFontSize;

    QVBoxLayout *vlayout;

  signals:
};

} // namespace Nutmeg

#endif // OPTIONSDISPLAYPANEL_H
