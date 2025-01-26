#include "flagcheckbox.h"

namespace Nutmeg {

FlagCheckbox::FlagCheckbox(std::shared_ptr<Flag> flag, QWidget *parent)
    : QWidget{parent}
    , mFlag(flag)
{}

} // namespace Nutmeg
