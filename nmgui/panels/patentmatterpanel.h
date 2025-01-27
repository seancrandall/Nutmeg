#ifndef PATENTMATTERPANEL_H
#define PATENTMATTERPANEL_H

#include <QObject>
#include <QWidget>
#include <QScrollArea>

#include "nutmeg.h"
#include "frame.h"
#include "groupbox.h"
#include "examinereditadd.h"
#include "lineedit.h"
#include "inventoreditadd.h"
#include "entitiessearchbox.h"
#include "inventorbutton.h"
#include "dateedit.h"
#include "addnewbutton.h"
#include "labeledwidgetleft.h"
#include "objects/patentmatter.h"
#include "windows/insertentitydialog.h"
#include "windows/insertpatentexaminerdialog.h"
#include "windows/insertinventordialog.h"

namespace Nutmeg {

class PatentMatterPanel : public Nutmeg::GroupBox
{
    Q_OBJECT
public:
    PatentMatterPanel(std::shared_ptr<PatentMatter> pmatter, QWidget *parent = nullptr);
    PatentMatterPanel(Key patentMatterId, QWidget *parent = nullptr);

public slots:
    void slotScatter(void);
    void slotGather(void);


protected:
    void LayoutWidgets(void);
    void ConnectSignalsAndSlots(void);
    // Controls
    //GroupBox *gb = new GroupBox("Patent Matter Data");
    LineEdit *cApplicationSerialNumber = new LineEdit();
    LineEdit *cConfirmationNumber = new LineEdit();
    LineEdit *cArtUnit = new LineEdit();
    LineEdit *cPatentNumber = new LineEdit();
    ExaminerEditAdd *cExaminer = new ExaminerEditAdd("Examiner");
    ExaminerEditAdd *cSupervisoryExaminer = new ExaminerEditAdd("Supervisory Examiner");
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
    LabeledWidgetLeft *lApplicant = new LabeledWidgetLeft("Applicant", cApplicant);
    LabeledWidgetLeft *lCriticalDate = new LabeledWidgetLeft("Critical Date", cCriticalDate);
    LabeledWidgetLeft *lBarDate = new LabeledWidgetLeft("Bar Date", cBarDate);
    LabeledWidgetLeft *lFilingDate = new LabeledWidgetLeft("Filing Date", cFilingDate);

protected slots:
    void slotInsertPatentExaminer(void);
    void slotInsertSupervisoryExaminer(void);
    void slotInsertFirstInventor(void);
    void slotInsertApplicant(void);

private:
    std::shared_ptr<PatentMatter> patMatter;
};

} // namespace Nutmeg

#endif // PATENTMATTERPANEL_H
