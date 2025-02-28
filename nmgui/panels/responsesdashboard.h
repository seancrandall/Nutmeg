#ifndef RESPONSESDASHBOARD_H
#define RESPONSESDASHBOARD_H

#include <QList>
#include <QLabel>
#include <QHBoxLayout>
#include <QScrollArea>

#include "frame.h"
#include "widgets/addnewbutton.h"
#include "responsedashpanel.h"

namespace Nutmeg {

class ResponsesDashboard : public Nutmeg::Frame
{
    Q_OBJECT
public:
    ResponsesDashboard(QWidget *parent=nullptr);

signals:
    void signalRefresh(void);

private:
    QList<ResponseDashPanel *> responseDashPanels;
    QLabel *responsesHeader;
    Nutmeg::AddNewButton *addRespButton;
    QHBoxLayout *responsesHeaderLayout;
    QScrollArea *responseScrollArea;
    Frame *responsesContainer;
    QVBoxLayout *mainResponseLayout;

    void SetupResponses(void);
    void ConnectSignalsAndSlots(void);

private slots:
    void AddNewResponse(void);
};

} // namespace Nutmeg

#endif // RESPONSESDASHBOARD_H
