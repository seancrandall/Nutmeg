#ifndef ENTITIESPANEL_H
#define ENTITIESPANEL_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

#include "frame.h"
#include "nutmeg.h"
#include "widgets/combobox.h"
#include "objects/task.h"
#include "objects/matter.h"
#include "dbaccess/viewparalegalsmodel.h"
#include "dbaccess/viewclientsmodel.h"
#include "dbaccess/viewworkattorneysmodel.h"

namespace Nutmeg
{

class EntitiesPanel : public Nutmeg::Frame
{
    Q_OBJECT
  public:
    EntitiesPanel(Key taskid, QWidget *parent=nullptr);

  protected slots:
    void updateClient(Key newkey);
    void updateParalegal(Key newkey);
    void updateAttorney(Key newkey);

  protected:
    Task task;
    Matter matter;

    ComboBox *clientsBox;
    ComboBox *paralegalsBox;
    ComboBox *attorneysBox;

    void SetupInterface(void);
    void ConnectSignalsAndSlots(void);
};

} // namespace Nutmeg

#endif // ENTITIESPANEL_H
