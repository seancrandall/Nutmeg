#ifndef APPOINTMENTSDASHBOARD_H
#define APPOINTMENTSDASHBOARD_H

#include <QList>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>

#include "nutmeg.h"
#include "frame.h"
#include "appointmentdashpanel.h"

namespace Nutmeg {

class AppointmentsDashboard : public Nutmeg::Frame
{
    Q_OBJECT
public:
    AppointmentsDashboard(QWidget *parent=nullptr);

signals:
    void signalRefresh(void);

private:
    QList<AppointmentDashPanel *>apptDashPanels;
    QLabel *apptsHeader;
    QHBoxLayout *apptsHeaderLayout;
    QScrollArea *apptsScrollArea;
    Frame *apptsContainer;
    QVBoxLayout *mainApptsLayout;

    void SetupAppointments(void);
};

} // namespace Nutmeg

#endif // APPOINTMENTSDASHBOARD_H
