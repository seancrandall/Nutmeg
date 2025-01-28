#include "titleedit.h"

namespace Nutmeg
{

TitleEdit::TitleEdit(Matter* matter, QWidget *parent)
    : TextEdit(parent)
    , mMatter(matter)
{
    setText(mMatter->Title);
}

} // namespace Nutmeg
