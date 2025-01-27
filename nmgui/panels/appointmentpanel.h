#ifndef APPOINTMENTPANEL_H
#define APPOINTMENTPANEL_H

#include <QObject>
#include <QWidget>

#include "widgets/frame.h"

namespace Nutmeg {

class AppointmentPanel : public Nutmeg::Frame
{
    Q_OBJECT
public:
    AppointmentPanel();
};

} // namespace Nutmeg

#endif // APPOINTMENTPANEL_H
