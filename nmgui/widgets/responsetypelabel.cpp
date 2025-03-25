#include "responsetypelabel.h"

namespace Nutmeg
{

ResponseTypeLabel::ResponseTypeLabel(std::shared_ptr<Response> response, QWidget *parent) : QLabel(parent)
{
    NUTMEGFONT;
    setFont(QFont("Arial", 14, QFont::Bold));

    QString labelText = "Other";
    QString objectType = response->objectType;

    if (objectType == "patentResponse")
        labelText = "Patent";

    if (objectType == "trademarkResponse")
        labelText = "Trademark";

    if (objectType == "copyrightResponse")
        labelText = "Copyright";

    setText(labelText);
}

} // namespace Nutmeg
