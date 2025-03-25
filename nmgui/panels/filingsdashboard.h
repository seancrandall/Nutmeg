#ifndef FILINGSDASHBOARD_H
#define FILINGSDASHBOARD_H

#include <QHBoxLayout>
#include <QScrollArea>
#include <QList>
#include <QLabel>

#include "widgets/addnewbutton.h"
#include "filingdashpanel.h"
#include "frame.h"

namespace Nutmeg {

class FilingsDashboard : public Nutmeg::Frame
{
    Q_OBJECT
public:
    FilingsDashboard(QWidget *parent=nullptr);

signals:
    void signalRefresh();

private slots:
    void AddNewFiling(void);

private:
    QList<FilingDashPanel *> filingDashPanels;
    QLabel *filingsHeader;
    Nutmeg::AddNewButton *addFilingButton;
    QHBoxLayout *filingsHeaderLayout;
    QScrollArea *filingsScrollArea;
    Frame *filingsContainer;
    QVBoxLayout *mainFilingsLayout;

    void SetupFilings(void);
    void ConnectSignalsAndSlots(void);
};

} // namespace Nutmeg

#endif // FILINGSDASHBOARD_H
