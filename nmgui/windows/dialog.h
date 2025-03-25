#ifndef NUTMEG_DIALOG_H
#define NUTMEG_DIALOG_H

#include "qdialogbuttonbox.h"
#include "widgets/combobox.h"
#include "widgets/dateedit.h"
#include "widgets/label.h"
#include "widgets/linedisplayid.h"
#include "widgets/lineedit.h"
#include "widgets/pushbutton.h"
#include "widgets/textedit.h"
#include "widgets/titleedit.h"
#include <QDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

#include "property.h"

namespace Nutmeg
{

class Dialog : public QDialog
{
    Q_OBJECT

  public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    Property(setParent, getParent) QWidget *parent;
    Property(setMinimumWidth, miniumWidth) int minWidth;
    Property(setMaximumWidth, maximumWidth) int maxWidth;
    Property(setMinimumHeight, minimumHeight) int minHeight;
    Property(setMaximumHeight, maximumHeight) int maxHeight;

  signals:
    // void accepted(void);
    // void rejected(void);
  public slots:
    virtual void slotScatter(void) = 0;
    virtual void slotGather(void) = 0;

    virtual void slotHandleRejected(void);
    virtual void slotHandleAccepted(void);

  protected:
    virtual void setupDisplay(void) = 0;
    virtual void connectSignalsAndSlots(void);

    ReadOnlyProperty(getWorkspace) QWidget *workspace;          /// Workspace should be the parent for all layouts. It
                                                                /// is the main area of the dialog
    ReadOnlyProperty(getButtonBox) QDialogButtonBox *buttonBox; /// Reference to the "Ok" and "Cancel" button box.

    QWidget *getWorkspace(void) const { return mWorkspace; }

    QDialogButtonBox *getButtonBox(void) const { return mButtonBox; }

    QDialogButtonBox *mButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QWidget *mWorkspace = new QWidget(this);
    QGridLayout *workLayout = new QGridLayout(); /// The layout we will use for the workspace widget.
                                                 /// Everything should be added to this

    void updateLayout(void); /// Should be called after all child layouts are finished
  private:
};

} // namespace Nutmeg
#endif // NUTMEG_DIALOG_H
