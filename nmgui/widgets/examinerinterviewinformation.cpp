#include "examinerinterviewinformation.h"

namespace Nutmeg
{

ExaminerInterviewInformation::ExaminerInterviewInformation(Key responseId, Key attorneyId)
    :   mResponse(Response (responseId))
      ,   mAttorney(Person(attorneyId))
{}

QString ExaminerInterviewInformation::getHtml()
{
    return GenerateHtml(mResponse.ResponseId, mAttorney.PersonId, mInterviewTime);
}

const QString ExaminerInterviewInformation::GenerateHtml(Key responseId, Key attorneyId, QDateTime interviewTime)
{
    Response response(responseId);
    Person attorney(attorneyId);
    PatentMatter matter(response.fkMatter);
    QString htmlDisplay;

    QString applicationNumber = matter.ApplicationSerialNumber;
    applicationNumber.remove(QRegularExpression("[/,]+"));

           // Convert interviewTime to Eastern Time
    QDateTime interviewTimeEastern = interviewTime.toTimeZone(QTimeZone("America/New_York"));

    htmlDisplay += "<html>";
    htmlDisplay += "<head><style>";
    htmlDisplay += "table { width: 100%; border-collapse: collapse; }";
    htmlDisplay += "td { padding: 8px; }";
    htmlDisplay += "th { font-weight: bold; text-align: left; }";
    htmlDisplay += "h1 { font-weight: bold; font-size: large; text-align: center; }";
    htmlDisplay += "</style></head>";
    htmlDisplay += "<body>";

           // Header
    htmlDisplay += "<h1>Examiner Interview for " + matter.AttorneyDocketNumber + "</h1>";

           // Start table
    htmlDisplay += "<table>";

           // Attorney Name
    htmlDisplay += "<tr><th>Attorney Name:</th><td>" + attorney.FirstName + " " + attorney.LastName + "</td></tr>";

           // S-signature
    htmlDisplay += "<tr><th>S-signature:</th><td>/" + attorney.FirstName + " " + attorney.LastName + "/</td></tr>";

           // Registration Number
    //htmlDisplay += "<tr><th>Registration Number:</th><td>" + attorney.RegistrationNumber + "</td></tr>";
    // At some future date, I need to make this an actual variable. But for now, just hard-coding it for myself.
    htmlDisplay += "<tr><th>Registration Number:</th><td>" + QString("57776") + "</td></tr>";

           // U.S. Application Number
    htmlDisplay += "<tr><th>U.S. Application Number:</th><td>" + applicationNumber + "</td></tr>";

           // Confirmation Number
    htmlDisplay += "<tr><th>Confirmation Number:</th><td>" + matter.ConfirmationNumber + "</td></tr>";

           // E-mail Address
    htmlDisplay += "<tr><th>E-mail Address:</th><td>" + attorney.PrimaryEmail + "</td></tr>";

           // Phone Number
    htmlDisplay += "<tr><th>Phone Number:</th><td>" + attorney.PrimaryPhone + "</td></tr>";

           // Proposed Interview Time
    htmlDisplay += "<tr><th>Proposed Interview Time:</th><td>" + interviewTimeEastern.toString("ddd MM-dd-yy hh:mm") + " (US Eastern Time)</td></tr>";

           // End table
    htmlDisplay += "</table>";

           // End HTML
    htmlDisplay += "</body></html>";

    return htmlDisplay;
}


} // namespace Nutmeg
