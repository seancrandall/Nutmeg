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
    void slotUpdateInterview(const QDateTime &newInterviewTime);



  private:
  protected:
    QDateTimeEdit *interviewTimeEditor;
    QLabel *interviewInformationPanel;
    QVBoxLayout *verticalLayout;

    virtual void setupDisplay(void) override;
    virtual void connectSignalsAndSlots(void) override;

    std::shared_ptr<Task> mTask;
    std::shared_ptr<Person> mAttorney;
    std::shared_ptr<PatentMatter> mMatter;
    std::shared_ptr<Person> mExaminer;
    QDateTime mAppointment = QDateTime::currentDateTime();

    ExaminerInterviewInformation eiInfo;
    bool valid = false;
};

} // namespace Nutmeg

#endif // ADDEXAMINERINTERVIEWDIALOG_H
