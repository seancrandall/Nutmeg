#ifndef ENTITYDIALOG_H
#define ENTITYDIALOG_H

#include "dialog.h"
#include "panels/entitypanel.h"

namespace Nutmeg {

class EntityDialog : public Nutmeg::Dialog
{
    Q_OBJECT
public:
    EntityDialog(Key entityId, QWidget *parent = nullptr);
    EntityDialog(std::shared_ptr<Entity> entity, QWidget *parent=nullptr);

protected:
    std::shared_ptr<Entity> mEntity;

    void setupDisplay(void) override;
    void connectSignalsAndSlots(void) override;
    void slotGather(void) override;
    void slotScatter(void) override;

    void LayoutWorkspace(void);

    EntityPanel *mPanel;

private:
    void Initialize(void);
};

} // namespace Nutmeg

#endif // ENTITYDIALOG_H
