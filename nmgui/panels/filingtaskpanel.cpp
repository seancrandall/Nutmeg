#include "filingtaskpanel.h"
#include "widgets/emailinventorsbutton.h"

namespace Nutmeg
{

FilingTaskPanel::FilingTaskPanel(std::shared_ptr<Filing> fil, QWidget *parent)
    : Nutmeg::AbstractTaskPanel(new EmailInventorsButton(fil), fil, parent)
{
    mFiling = fil;
    mMatter = std::make_shared<Matter>(mFiling->fkMatter);
}

void FilingTaskPanel::slotExtraButtonAction()
{
    QString body;

    body = QString("Hello!\n\nThis is Sean Crandall with Patent Capital Group. I'm the attorney who will ");
    body = body % QString("be helping you with your patent application for the above case. I'd like to ");
    body = body % QString("set up a time to discuss your invention. Can you please let me know some times ");
    body = body % QString("in the next week or so when you would be available?\n\nThanks!");

    emit signalEmailInventors(body);
}

} // namespace Nutmeg
