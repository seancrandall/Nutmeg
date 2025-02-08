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


protected:
    std::shared_ptr<PatentMatter> mPatentMatter;

    AddNewButton *cAddInventor;
    QList<InventorButton *> cInventorButtons;


private:
    void Initialize(void);
    void LoadInventors(void);
    void InitializeObjects(void);
    void LayoutWidgets(void);
    QList<Key> mInventorList;
};

} // namespace Nutmeg

#endif // INVENTORSPANEL_H
