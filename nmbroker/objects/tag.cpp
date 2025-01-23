#include "tag.h"

namespace Nutmeg
{

Tag::Tag(String tag_text) : mTagText{tag_text} { mTagId = Nutdb::GetTagId(mTagText); }

Tag::Tag(Key tag_id) : mTagId(tag_id)
{
    TagData dat;
    dat = Nutdb::GetTag(mTagId);
    mTagText = dat.TagText;
}

} // namespace Nutmeg
