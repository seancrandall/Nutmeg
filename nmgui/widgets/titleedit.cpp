#include "titleedit.h"

namespace Nutmeg
{

TitleEdit::TitleEdit(std::shared_ptr<Matter>matter, QWidget *parent)
    : TextEdit(parent)
    , mMatter(matter)
{
    setText(mMatter->Title);
}

} // namespace Nutmeg
