#ifndef FLAGCHECKBOX_H
#define FLAGCHECKBOX_H

#include <QWidget>
#include <QCheckBox>
#include <QLabel>

#include "nutmeg.h"
#include "objects/flag.h"
#include "property.h"

namespace Nutmeg {

typedef enum{
     Horizontal
    ,Vertical
} CBoxOrientation;

class FlagCheckbox : public QWidget
{
    Q_OBJECT
public:
    explicit FlagCheckbox(std::shared_ptr<Flag> flag, QWidget *parent = nullptr);

    Property(getValue, setValue) bool value;
    Property(getOrientation, setOrientation) CBoxOrientation orientation;
    ReadOnlyProperty(getLabel) QString label;
    ReadOnlyProperty(getTooltip) QString tooltip;


    bool getValue(void) const;
    void setValue(bool newvalue) const;
    CBoxOrientation getOrientation(void) const;
    void setOrientation(CBoxOrientation neworientation);
    const QString getLabel(void) const;
    const QString getTooltip(void) const;
signals:

protected:
    std::shared_ptr<Flag> mFlag;
    bool mValue;
    CBoxOrientation mOrientation;
    QString mLabel;
    QString mTooltip;
};

} // namespace Nutmeg

#endif // FLAGCHECKBOX_H
