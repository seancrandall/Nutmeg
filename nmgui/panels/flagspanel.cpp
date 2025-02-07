#include "flagspanel.h"

namespace Nutmeg
{

FlagsPanel::FlagsPanel(Key objectid, QWidget *parent)
    : Frame(parent)
    , mObject(std::make_shared<Object>(objectid))
{
    SetupPanel();
}

void FlagsPanel::SetupPanel()
{
    QList<FlagData> flagList = mObject->flags;
    QGridLayout *layout = new QGridLayout(this);

    // Calculate the approximate square root to determine grid size
    int count = flagList.size();
    int columns = static_cast<int>(std::sqrt(count));
    //int rows = (count + columns - 1) / columns; // Ceiling division

    for (int i = 0; i < count; ++i) {
        FlagCheckbox *checkBox = new FlagCheckbox(flagList[i].CamelCase, mObject->getId());
        // Add checkbox to the grid
        layout->addWidget(checkBox, i / columns, i % columns);
    }

    // Optionally, you can add some spacing or margins
    layout->setSpacing(2);
    //layout->setContentsMargins(4, 4, 4, 4);

    // Set the layout to this panel
    this->setLayout(layout);
}
} // namespace Nutmeg
