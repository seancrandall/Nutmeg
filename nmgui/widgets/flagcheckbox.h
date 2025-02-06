#ifndef FLAGCHECKBOX_H
#define FLAGCHECKBOX_H

#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QGridLayout>

#include "objects/object.h"
#include "property.h"
#include "panels/frame.h"
#include "checkbox.h"
#include "label.h"

namespace Nutmeg {

/**
 * @brief CBoxOrientation
 * Is the label above, below, right, or left
 */
typedef enum{
     CheckLabelTop
    , CheckLabelBottom
    , CheckLabelLeft
    , CheckLabelRight
} CBoxOrientation;

class FlagCheckbox : public Frame
{
    Q_OBJECT
public:
    explicit FlagCheckbox(FlagData flag, std::shared_ptr<Object> object, QWidget *parent = nullptr);
    explicit FlagCheckbox(QString camelCase, Key objectId, QWidget *parent = nullptr);
    explicit FlagCheckbox();

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
    std::shared_ptr<Object> mObject;
    FlagData mFlag;
    CBoxOrientation mOrientation;

    QGridLayout *layout;
    CheckBox *cBox;
    Label *cLabel;

    void LayoutCheckbox(void);

protected slots:
    void UpdateValue(void);
};

} // namespace Nutmeg

#endif // FLAGCHECKBOX_H
