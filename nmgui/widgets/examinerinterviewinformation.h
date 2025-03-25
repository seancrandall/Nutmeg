#ifndef EXAMINERINTERVIEWINFORMATION_H
#define EXAMINERINTERVIEWINFORMATION_H

#include <QTimeZone>
#include <QRegularExpression>

#include "objects/response.h"
#include "objects/person.h"
#include "objects/patentmatter.h"
#include "property.h"

namespace Nutmeg
{

class ExaminerInterviewInformation
{
  public:
    ExaminerInterviewInformation(Key responseId, Key attorneyId);

    Property(getInterviewTime, setInterviewTime) QDateTime interviewTime;
    ReadOnlyProperty(getHtml) QString html;

    QString getHtml(void);
    const QDateTime getInterviewTime(void) const {return mInterviewTime;};
    void setInterviewTime(const QDateTime &newDateTime) {mInterviewTime = newDateTime;};

    const static QString GenerateHtml(const Key responseId, const Key attorneyId, const QDateTime interviewTime);

  private:
    Response mResponse;
    Person mAttorney;
    QDateTime mInterviewTime = QDateTime::currentDateTime();
};


} // namespace Nutmeg

#endif // EXAMINERINTERVIEWINFORMATION_H
