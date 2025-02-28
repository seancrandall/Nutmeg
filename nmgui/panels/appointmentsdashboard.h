#ifndef APPOINTMENTSDASHBOARD_H
#define APPOINTMENTSDASHBOARD_H

#include "frame.h"

namespace Nutmeg {

class AppointmentsDashboard : public Nutmeg::Frame
{
    Q_OBJECT
public:
    AppointmentsDashboard(QWidget *parent=nullptr);
};

} // namespace Nutmeg

#endif // APPOINTMENTSDASHBOARD_H
