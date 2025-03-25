#ifndef EXAMINERINFOPANEL_H
#define EXAMINERINFOPANEL_H

#include "nutmeg.h"
#include "objects/person.h"
#include "property.h"
#include "frame.h"
#include "widgets/labeledwidgetleft.h"
#include "widgets/lineedit.h"
#include "widgets/pushbutton.h"

namespace Nutmeg {

class ExaminerInfoPanel : public Nutmeg::Frame
{
    Q_OBJECT
public:
    ExaminerInfoPanel(Key examinerId, QWidget *parent=nullptr);
    ExaminerInfoPanel(std::shared_ptr<Person> pExaminer, QWidget *parent=nullptr);

signals:
    void signalEmailExaminer(void);

protected:
    //Examiner Information
    PushButton *bExaminerName;
    LineEdit *cExaminerPhoneNumber;
    LineEdit *cExaminerEmail;
    PushButton *bEmailExaminer;
    LabeledWidgetLeft *lExaminerPhoneNumber
        ,*lExaminerEmail;

    std::shared_ptr<Person> mExaminer;

protected slots:
    void emailExaminer(void);
    void changeEmail(void);
    void changePhoneNumber(void);
    void openExaminerInfo(void);

private:
    void Initialize();
    void ConnectSignalsAndSlots();
    void InitializeControls();
    void LayoutControls();

};

} // namespace Nutmeg

#endif // EXAMINERINFOPANEL_H
