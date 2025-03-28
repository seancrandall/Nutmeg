#ifndef INVENTORSPANEL_H
#define INVENTORSPANEL_H

#include "nutmeg.h"
#include "property.h"
#include "objects/patentmatter.h"
#include "widgets/addnewbutton.h"
#include "widgets/inventorbutton.h"

#include "frame.h"

namespace Nutmeg {

class InventorsPanel : public Nutmeg::Frame
{
    Q_OBJECT
public:
    explicit InventorsPanel(QWidget *parent = nullptr);
    explicit InventorsPanel(Key patentMatterId, QWidget *parent=nullptr);
    explicit InventorsPanel(std::shared_ptr<PatentMatter> patentMatter, QWidget *parent=nullptr);

signals:
    void inventorsChanged(void);

protected:
    std::shared_ptr<PatentMatter> mPatentMatter;

    AddNewButton *cAddInventor;
    QList<InventorButton *> cInventorButtons;

protected slots:
    void AddInventor();
    void RemoveInventor(Key inventorId);
    void OpenInventor(Key inventorId);

private:
    void Initialize(void);
    void LoadInventors(void);
    void InitializeObjects(void);
    void LayoutWidgets(void);
    void ConnectSignalsAndSlots(void);
    QList<Key> mInventorList;
    void clearLayout(void);
};

} // namespace Nutmeg

#endif // INVENTORSPANEL_H
