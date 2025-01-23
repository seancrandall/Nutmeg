#ifndef NUTMEG_TAG_H
#define NUTMEG_TAG_H

#include "dbaccess/nutdb.h"
#include "nutmeg.h"
#include "property.h"

namespace Nutmeg
{

class Tag
{
  public:
    Tag(String tag_text);
    Tag(Key tag_id);

    ReadOnlyProperty(getTagText) String tagText;
    ReadOnlyProperty(getTagId) Key tagId;

    String getTagText(void) { return mTagText; }

    Key getTagId(void) { return mTagId; }

  protected:
    Key mTagId;
    String mTagText;
};

} // namespace Nutmeg

#endif // NUTMEG_TAG_H
