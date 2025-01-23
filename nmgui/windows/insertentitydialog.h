#ifndef NUTMEG_INSERTENTITYDIALOG_H
#define NUTMEG_INSERTENTITYDIALOG_H

#include "insertdialog.h"
#include <QObject>
#include <QWidget>

#include "widgets/insertenterprisepanel.h"
#include "widgets/insertpersonpanel.h"

// #include "widgets/combobox.h"

namespace Nutmeg
{

typedef enum
{
    NaturalPerson = 0,
    Enterprise = 1
} EntityType;

class InsertEntityDialog : public Nutmeg::InsertDialog
{
    Q_OBJECT
  public:
    explicit InsertEntityDialog(QWidget *parent = nullptr);
    explicit InsertEntityDialog(const QString entityName, QWidget *parent = nullptr);
    explicit InsertEntityDialog(const QString firstName, const QString lastName, QWidget *parent = nullptr);

    InsertPersonPanel *ppanel = new InsertPersonPanel();
    InsertEnterprisePanel *epanel = new InsertEnterprisePanel();
    QComboBox *cType = new QComboBox();

    LabeledWidgetLeft *lType = new LabeledWidgetLeft("Entity Type", cType);

    QWidget *container = new QWidget();
    QHBoxLayout *hlayout = new QHBoxLayout();

  public slots:
    virtual void slotHandleAccepted(void) override;
    // virtual void slotHandleRejected(void) override;

  protected:
    virtual void connectSignalsAndSlots(void) override;
    virtual void setupDisplay(void) override;

  protected slots:
    void slotChangeType(int entityType);
};

} // namespace Nutmeg

#endif // NUTMEG_INSERTENTITYDIALOG_H
