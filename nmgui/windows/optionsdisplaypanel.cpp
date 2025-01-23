#include "optionsdisplaypanel.h"

namespace Nutmeg
{

OptionsDisplayPanel::OptionsDisplayPanel(QWidget *parent) : QWidget{parent}
{
    eFontFamily = new QFontComboBox();
    eFontSize = new QSpinBox();

    wFontFamily = new LabeledWidgetLeft("Font Family", eFontFamily);
    wFontSize = new LabeledWidgetLeft("Font Size", eFontSize);

    vlayout = new QVBoxLayout(this);

    vlayout->addLayout(wFontFamily);
    vlayout->addLayout(wFontSize);
    vlayout->addStretch();
    adjustSize();
}

void OptionsDisplayPanel::setFontFamily(const QString newfam)
{
    QFont newfont;
    newfont.setFamily(newfam);
    eFontFamily->setCurrentFont(newfont);
}

} // namespace Nutmeg
