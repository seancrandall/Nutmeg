#ifndef NUTMEG_PATENTMATTERDIALOG_H
#define NUTMEG_PATENTMATTERDIALOG_H

#include "matterdialog.h"
#include "nutmeg.h"
#include "property.h"
#include <QObject>
#include <QWidget>

#include "objects/patentmatter.h"

#include "widgets/addnewbutton.h"
#include "widgets/dateedit.h"
#include "widgets/entitiessearchbox.h"
#include "widgets/examinereditadd.h"
#include "widgets/examinersearchbox.h"
#include "widgets/groupbox.h"
#include "widgets/inventoreditadd.h"
#include "widgets/inventorssearchbox.h"
#include "widgets/plusbutton.h"

namespace Nutmeg
{

class PatentMatterDialog : public Nutmeg::MatterDialog
{
    Q_OBJECT
  public:
    PatentMatterDialog(Key pmkey, QWidget *parent = nullptr);

  protected:
    // Controls
    GroupBox *gb = new GroupBox("Patent Matter Data");
    LineEdit *cApplicationSerialNumber = new LineEdit();
    LineEdit *cConfirmationNumber = new LineEdit();
    LineEdit *cArtUnit = new LineEdit();
    LineEdit *cPatentNumber = new LineEdit();
    // ExaminerSearchBox *cExaminer = new ExaminerSearchBox();
    // PlusButton *cpbInsertExaminer = new PlusButton();
    ExaminerEditAdd *cExaminer = new ExaminerEditAdd("Examiner");
    // ExaminerSearchBox *cSupervisoryExaminer = new ExaminerSearchBox();
    // PlusButton *cpbInsertSupervisoryExaminer = new PlusButton();
    ExaminerEditAdd *cSupervisoryExaminer = new ExaminerEditAdd("Supervisory Examiner");
    // InventorsSearchBox *cFirstInventor = new InventorsSearchBox();
    // PlusButton *cpbInsertFirstInventor = new PlusButton();
    InventorEditAdd *cFirstInventor = new InventorEditAdd("First Named Inventor");
    EntitiesSearchBox *cApplicant = new EntitiesSearchBox();
    PlusButton *cpbInsertApplicant = new PlusButton();

    // Date Controls
    QGroupBox *dateGroup = new QGroupBox("Dates");
    DateEdit *cCriticalDate = new DateEdit(nullptr);
    DateEdit *cBarDate = new DateEdit(nullptr);
    DateEdit *cFilingDate = new DateEdit(nullptr);
    DateEdit *cDateIssued = new DateEdit(nullptr);

    // Inventor Box
    QGroupBox *inventorBox = new QGroupBox("Inventor");
    QWidget *ibFrame = new QWidget();
    QVBoxLayout *iblayout = new QVBoxLayout();
    AddNewButton *canInsertInventor = new AddNewButton();

    // Labeling
    LabeledWidgetLeft *lApplicationSearialNumber =
        new LabeledWidgetLeft("Application Serial Number", cApplicationSerialNumber);
    LabeledWidgetLeft *lConfirmationNumber = new LabeledWidgetLeft("Confirmation Number", cConfirmationNumber);
    LabeledWidgetLeft *lArtUnit = new LabeledWidgetLeft("Art Unit", cArtUnit);
    LabeledWidgetLeft *lPatentNumber = new LabeledWidgetLeft("Patent Number", cPatentNumber);
    // LabeledWidgetLeft *lExaminer = new LabeledWidgetLeft("Examiner",
    // cExaminer); LabeledWidgetLeft *lSupervisoryExaminer = new
    // LabeledWidgetLeft("Supervisory Examiner", cSupervisoryExaminer);
    // LabeledWidgetLeft *lFirstInventor = new LabeledWidgetLeft("First Named
    // Inventor", cFirstInventor);
    LabeledWidgetLeft *lApplicant = new LabeledWidgetLeft("Applicant", cApplicant);
    LabeledWidgetLeft *lCriticalDate = new LabeledWidgetLeft("Critical Date", cCriticalDate);
    LabeledWidgetLeft *lBarDate = new LabeledWidgetLeft("Bar Date", cBarDate);
    LabeledWidgetLeft *lFilingDate = new LabeledWidgetLeft("Filing Date", cFilingDate);

    PatentMatter pmatter;

    virtual void slotScatter(void) override;
    virtual void slotGather(void) override;
    virtual void connectSignalsAndSlots(void) override;

  protected slots:
    void slotInsertPatentExaminer(void);
    void slotInsertSupervisoryExaminer(void);
    void slotInsertFirstInventor(void);
    void slotInsertApplicant(void);

  private:
    void LayoutWidgets(void);
};

} // namespace Nutmeg

#endif // NUTMEG_PATENTMATTERDIALOG_H
