#include "flagcheckbox.h"

namespace Nutmeg {

FlagCheckbox::FlagCheckbox(FlagData flag, std::shared_ptr<Object> object, QWidget *parent)
    : Frame(parent)
{
    mObject = object;
    mFlag = flag;
    LayoutCheckbox();
}

FlagCheckbox::FlagCheckbox(QString camelCase, Key objectId, QWidget *parent)
    : Frame(parent)
{
    mObject = std::make_shared<Object>(objectId);
    mFlag = mObject->flagData[camelCase];
    LayoutCheckbox();
}

FlagCheckbox::FlagCheckbox()
    : Frame(nullptr)
    , mObject(nullptr)
    , mFlag(FlagData())
{
    LayoutCheckbox();
}

void FlagCheckbox::LayoutCheckbox()
{
    layout = new QGridLayout(this);
    cBox = new CheckBox();
    cLabel = new Label(mFlag.Label);

    cBox->setChecked(mFlag.FlagValue); // Assuming FlagValue indicates initial state
    cLabel->setToolTip(mFlag.Description); // Set tooltip from description

    switch(mOrientation) {
    case CheckLabelLeft:
        layout->addWidget(cLabel, 0, 0);
        layout->addWidget(cBox, 0, 1);
        break;
    case CheckLabelRight:
        layout->addWidget(cBox, 0, 0);
        layout->addWidget(cLabel, 0, 1);
        break;
    case CheckLabelTop:
        layout->addWidget(cLabel, 0, 0);
        layout->addWidget(cBox, 1, 0);
        break;
    case CheckLabelBottom:
        layout->addWidget(cBox, 0, 0);
        layout->addWidget(cLabel, 1, 0);
        break;
    }

    QObject::connect(cBox, &QCheckBox::checkStateChanged,
                     this, &FlagCheckbox::UpdateValue);
}

void FlagCheckbox::UpdateValue(bool newValue)
{
    mFlag.FlagValue = newValue;
    if(mObject)
        mObject->flag[mFlag.CamelCase] = newValue;
}

} // namespace Nutmeg
