#include "maindashboard.h"

namespace Nutmeg {


MainDashboard::MainDashboard(QWidget *parent)
    : QSplitter(parent)
{
    setOrientation(Qt::Vertical);
    setChildrenCollapsible(true);
}

} // namespace Nutmeg
