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

ResponseTypeLabel::ResponseTypeLabel(const responsesDashboardEntry &entry, QWidget *parent)
    : QLabel(parent)
{
    NUTMEGFONT;
    setFont(QFont("Arial", 14, QFont::Bold));

    QString labelText = "Other";
    QString className = entry.getTaskClassName();

    if (className == "patentResponse")
        labelText = "Patent";

    if (className == "trademarkResponse")
        labelText = "Trademark";

    if (className == "copyrightResponse")
        labelText = "Copyright";

    setText(labelText);
}

} // namespace Nutmeg
