#ifndef NUTMEG_COMBOEDITADD_H
#define NUTMEG_COMBOEDITADD_H

#include <QHBoxLayout>
#include <QObject>
#include <QWidget>

#include "widgets/combobox.h"
#include "widgets/editbutton.h"
#include "widgets/label.h"
#include "widgets/plusbutton.h"

namespace Nutmeg
{

class ComboEditAdd : public QWidget
{
    Q_OBJECT
  public:
    explicit ComboEditAdd(QWidget *parent = nullptr);
    explicit ComboEditAdd(QString label, QWidget *parent = nullptr);

    // Properties
    Property(getKey, setKey) Key key;
    Property(getLabelText, setLabelText) QString labelText;
    Property(getCurrentText, setCurrentText) QString text;
    Property(getModel, setModel) TableModel *model;
    Property(getColumn, setColumn) int column;
    Property(getIsNew, setIsNew) bool isNew;

    Key getKey(void) { return cCombo->getKey(); }

  signals:
    void signalSelected(Key newkey);
    void signalAddNew(void);
    void signalEdit(Key key);

  public slots:

    void setTableModel(TableModel *newmod) { cCombo->setTableModel(newmod); }

    void setToKey(Key newkey) { cCombo->setToKey(newkey); }

    void setColumn(int newcol) { cCombo->setColumn(newcol); }

    void refresh(void) { cCombo->refresh(); }

    void setKey(Key newkey) { cCombo->setToKey(newkey); }

    void setLabelText(QString ltext)
    {
        cLabel->setText(ltext);
        cLabel->show();
    }

    void setCurrentText(QString newtext) { cCombo->setCurrentText(newtext); }

    void setModel(TableModel *model) { cCombo->setTableModel(model); }

  protected:
    QHBoxLayout *layout = nullptr;
    Label *cLabel = new Label();
    ComboBox *cCombo = new ComboBox();
    EditButton *cEdit = new EditButton();
    PlusButton *cPlus = new PlusButton();

    void setupDisplay(void);
    void connectSignalsAndSlots(void);

  protected slots:
    void slotChangeIndex(void);
    void slotOpen(void);
    void slotNew(void);
};

} // namespace Nutmeg

#endif // NUTMEG_COMBOEDITADD_H
