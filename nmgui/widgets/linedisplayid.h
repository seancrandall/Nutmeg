#ifndef NUTMEG_LINEDISPLAYID_H
#define NUTMEG_LINEDISPLAYID_H

#include "widgets/lineedit.h"

namespace Nutmeg
{

class LineDisplayId : public Nutmeg::LineEdit
{
  public:
    LineDisplayId(QWidget *parent = nullptr);
    LineDisplayId(Key displayKey, QWidget *parent = nullptr);

    Property(getId, setId) Key id;

    Key getId(void);
    void setId(Key newid);
};

} // namespace Nutmeg

#endif // NUTMEG_LINEDISPLAYID_H
