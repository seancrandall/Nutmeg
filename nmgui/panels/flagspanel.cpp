#include "flagspanel.h"

namespace Nutmeg
{

FlagsPanel::FlagsPanel(Key objectid, QWidget *parent)
    : Frame(parent)
      , object(objectid)
{
    QGridLayout *layout = new QGridLayout(this);
    QLabel *label = new QLabel("FLAGS");

    layout->addWidget(label);
}

} // namespace Nutmeg
