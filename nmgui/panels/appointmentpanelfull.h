#ifndef APPOINTMENTPANELFULL_H
#define APPOINTMENTPANELFULL_H

#include <QObject>
#include <QWidget>
#include "appointmentpanel.h"

namespace Nutmeg {

class AppointmentPanelFull : public Nutmeg::AppointmentPanel
{
    Q_OBJECT
public:
    AppointmentPanelFull();

    //Need to add Notes
    //Flags
    //Documents
};

} // namespace Nutmeg

#endif // APPOINTMENTPANELFULL_H
