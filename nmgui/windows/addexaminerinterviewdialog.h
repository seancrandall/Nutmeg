#ifndef ADDEXAMINERINTERVIEWDIALOG_H
#define ADDEXAMINERINTERVIEWDIALOG_H

#include <QObject>
#include <QWidget>
#include <QUrl>
#include <QDesktopServices>

#include "nutmeg.h"
#include "property.h"
#include "dialog.h"
#include "widgets/warningbox.h"
#include "widgets/examinerinterviewinformation.h"
#include "widgets/dateedit.h"
#include "widgets/timeedit.h"

#include "settings.h"

namespace Nutmeg
{

class AddExaminerInterviewDialog : public Nutmeg::Dialog
{
    Q_OBJECT
  public:
    AddExaminerInterviewDialog(Key responseId, QWidget *parent=nullptr);


  protected slots:
    virtual void slotHandleRejected(void) override;
    virtual void slotHandleAccepted(void) override;
    virtual void slotScatter(void) override;
    virtual void slotGather(void) override;
    //void slotUpdateInterview();

    void slotTimeChanged();
    void slotDateChanged();



  protected:
    //QDateTimeEdit *interviewTimeEditor;
    DateEdit *dateEditor;
    TimeEdit *timeEditor;
    QLabel *interviewInformationPanel;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *dtLayout;

    virtual void setupDisplay(void) override;
    virtual void connectSignalsAndSlots(void) override;

    std::shared_ptr<Task> mTask;
    std::shared_ptr<Person> mAttorney;
    std::shared_ptr<PatentMatter> mMatter;
    std::shared_ptr<Person> mExaminer;

    ExaminerInterviewInformation eiInfo;
    bool valid;

  private:
    QDate mAppointmentDate;
    QTime mAppointmentTime;
    QDateTime mAppointmentDateTime;
};

} // namespace Nutmeg

#endif // ADDEXAMINERINTERVIEWDIALOG_H
