#include "optionsdocumentpanel.h"

namespace Nutmeg
{

OptionsDocumentPanel::OptionsDocumentPanel(QWidget *parent) : QWidget{parent}
{
    eDocsLocation = new LineEdit();
    eMaxFilenameLength = new QSpinBox();
    eMaxFilenameLength->setMaximum(65535);
    eMaxFilenameLength->setMinimum(1);

    wDocsLocation = new LabeledWidgetLeft("Documents Location", eDocsLocation);
    wMaxFilenameLength = new LabeledWidgetLeft("Maximum Filename Size", eMaxFilenameLength);

    vlayout = new QVBoxLayout(this);

    vlayout->addLayout(wDocsLocation);
    vlayout->addLayout(wMaxFilenameLength);
    vlayout->addStretch();
    adjustSize();
}

} // namespace Nutmeg
