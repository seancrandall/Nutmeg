#ifndef FILINGTASKPANEL_H
#define FILINGTASKPANEL_H

#include <QObject>
#include <QWidget>

#include "objects/filing.h"
#include "abstracttaskpanel.h"

namespace Nutmeg
{

class FilingTaskPanel : public Nutmeg::AbstractTaskPanel
{
    Q_OBJECT
public:
    FilingTaskPanel(std::shared_ptr<Filing> fil, QWidget *parent = nullptr);

signals:
    void signalSomethingChanged(void);
    void signalEmailInventors(const QString& body);

public slots:
    virtual void slotExtraButtonAction(void) override;

protected:
    std::shared_ptr<Filing> mFiling;
    std::shared_ptr<Matter> mMatter;
};

} // namespace Nutmeg

#endif // FILINGTASKPANEL_H
