#ifndef FILINGSDASHBOARD_H
#define FILINGSDASHBOARD_H

#include "frame.h"

namespace Nutmeg {

class FilingsDashboard : public Nutmeg::Frame
{
    Q_OBJECT
public:
    FilingsDashboard(QWidget *parent=nullptr);
};

} // namespace Nutmeg

#endif // FILINGSDASHBOARD_H
