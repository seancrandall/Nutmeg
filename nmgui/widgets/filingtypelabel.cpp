#include "filingtypelabel.h"

namespace Nutmeg
{

FilingTypeLabel::FilingTypeLabel(Filing* filing, QWidget *parent)
    : QLabel(parent)
{
    NUTMEGFONT;
    setFont(QFont("Arial", 14, QFont::Bold));

    QString labelText = "Other";
    QString objectType = filing->objectType;

    if (objectType == "patentFiling")
        labelText = "Patent";

    if (objectType == "trademarkFiling")
        labelText = "Trademark";

    if (objectType == "CopyrightFiling")
        labelText = "Copyright";

    setText(labelText);
}

} // namespace Nutmeg
