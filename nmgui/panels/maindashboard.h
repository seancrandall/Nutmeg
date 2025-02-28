#ifndef MAINDASHBOARD_H
#define MAINDASHBOARD_H

#include <QSplitter>
#include <QObject>
#include <QWidget>

#include "frame.h"
#include "filingsdashboard.h"
#include "responsesdashboard.h"
#include "appointmentsdashboard.h"

namespace Nutmeg {

class MainDashboard : public QSplitter
{
    Q_OBJECT
public:
    MainDashboard(QWidget *parent=nullptr);

signals:
    void signalRefresh(void);

public slots:
    void Refresh(void);

private:
    AppointmentsDashboard *appts;
    FilingsDashboard *filings;
    ResponsesDashboard *resps;

    void Setup(void);
};

} // namespace Nutmeg

#endif // MAINDASHBOARD_H
