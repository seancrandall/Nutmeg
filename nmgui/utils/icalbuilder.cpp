#include "icalbuilder.h"
#include "logger.h" //Provides Logger::LogMessage(const QString& message)

namespace Nutmeg {

IcalBuilder::IcalBuilder()
{
    Initialize();
}

IcalBuilder::IcalBuilder(const QDateTime &apptTime, QDateTime apptEnd)
{
    Initialize();

    if (apptTime.isValid()) {
        startTime = apptTime;
    } else {
        Logger::LogMessage("Warning: Invalid start time provided. Using defaults.");
        return;
    }

    //If apptEnd is before the start time, just make them equal and return;
    if( (apptEnd.isValid()) && (apptEnd < apptTime) )
    {
        endTime = apptTime;
        QString logmessage = "Warning: Tried to create an appointment with an end time before start time.\n";
        logmessage += "\n Start time was " + apptTime.toString(Qt::ISODate);
        logmessage += ", while end time was " + apptEnd.toString(Qt::ISODate) + ".";
        Logger::LogMessage(logmessage);
        endTime = startTime;
        return;
    }

    if (apptEnd.isValid()) {
        endTime = apptEnd;
    } else {
        endTime = startTime.addSecs(1800); //default to 30 minutes
    }
}

IcalBuilder::~IcalBuilder()
{
    // No dynamic memory to clean up, but good practice to have a destructor
}

const QString IcalBuilder::getEventName() const
{
    return mEventName;
}

void IcalBuilder::setEventName(const QString &newname)
{
    if (newname.isEmpty()) {
        Logger::LogMessage("Warning: Empty event name set.");
    }
    mEventName = newname;
}

const QDateTime IcalBuilder::getStartTime() const
{
    return mStartTime;
}

void IcalBuilder::setStartTime(const QDateTime &newtime)
{
    if (!newtime.isValid()) {
        Logger::LogMessage("Error: Attempt to set invalid start time.");
        return;
    }
    mStartTime = newtime;
    // Convert QDateTime to iCalendar format YYYYMMDDTHHMMSSZ for UTC
    if (newtime.timeSpec() == Qt::UTC) {
        mStartTimeString = newtime.toString("yyyyMMddThhmmssZ");
    } else {
        // If it's not UTC, convert it to UTC first
        QDateTime utcTime = newtime.toUTC();
        mStartTimeString = utcTime.toString("yyyyMMddThhmmssZ");
    }
}

const QDateTime IcalBuilder::getEndTime() const
{
    return mEndTime;
}

void IcalBuilder::setEndTime(const QDateTime &newtime)
{
    if (!newtime.isValid()) {
        Logger::LogMessage("Error: Attempt to set invalid end time.");
        return;
    }
    mEndTime = newtime;
    if (newtime.timeSpec() == Qt::UTC) {
        mEndTimeString = newtime.toString("yyyyMMddThhmmssZ");
    } else {
        // If it's not UTC, convert it to UTC first
        QDateTime utcTime = newtime.toUTC();
        mEndTimeString = utcTime.toString("yyyyMMddThhmmssZ");
    }
}

const QString IcalBuilder::getCompanyName() const
{
    return mCompanyName;
}

void IcalBuilder::setCompanyName(const QString &newname)
{
    if (newname.isEmpty()) {
        Logger::LogMessage("Warning: Empty company name set.");
    }
    mCompanyName = newname;
}

const QString IcalBuilder::getProductName() const
{
    return mProductName;
}

void IcalBuilder::setProductName(const QString &newname)
{
    if (newname.isEmpty()) {
        Logger::LogMessage("Warning: Empty product name set.");
    }
    mProductName = newname;
}

QString IcalBuilder::getIcsString()
{
    if (mStartTimeString.isEmpty() || mEndTimeString.isEmpty() || mEventName.isEmpty()) {
        Logger::LogMessage("Error: Cannot generate ICS string. Missing necessary data.");
        return QString();
    }
    mIcsFileString = createIcsEvent(mEventName
                                    , mStartTimeString
                                    , mEndTimeString
                                    , mCompanyName
                                    , mProductName);

    return mIcsFileString;
}

void IcalBuilder::openIcsFile()
{
    QString icsString = getIcsString();
    if (icsString.isEmpty()) {
        Logger::LogMessage("Error: No ICS string to write to file.");
        return;
    }

    // Use QStandardPaths to get a temporary location
    QString tempDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    if (tempDir.isEmpty()) {
        Logger::LogMessage("Error: Could not find a writable temporary directory.");
        return;
    }

    // Create a unique file name
    QString fileName = tempDir + QDir::separator() + "event_" + QUuid::createUuid().toString() + ".ics";

    // Write out the ICS file
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << icsString;
        file.close();

        // Use QDesktopServices to open the ICS file with the default handler
        if (!QDesktopServices::openUrl(QUrl::fromLocalFile(fileName))) {
            // Handle error - file could not be opened by default application
            Logger::LogMessage("Error: Failed to open ICS file with default application.");
        }
    } else {
        // Handle error - file could not be created
        Logger::LogMessage("Error: Failed to create ICS file at: " + fileName);
    }
}
/**
    @brief IcalBuilder::createIcsEvent Static method to build an ICS file string from several input parameters
    @param summary The "summary" or heder description of the event.
    @param start Start time for the event (defaults to current date/time if not specified)
    @param end End time for the event (defaults to 30 minutes after start)
    @param companyName Name of the company that provides the ICS file
    @param productName Name of the product used to generate the ICS file
    @return Returns a QString with the ICS file contents
     */
QString IcalBuilder::createIcsEvent(const QString &summary
                                    , const QString &start
                                    , const QString &end
                                    , const QString &companyName
                                    , const QString &productName)
{
    icalcomponent *cal = icalcomponent_new(ICAL_VCALENDAR_COMPONENT);
    if (!cal) {
        Logger::LogMessage("Error: Failed to create VCALENDAR component.");
        return QString();
    }

    icalcomponent_add_property(cal, icalproperty_new_version("2.0"));

    QString ProductId;
    QString lCompanyName = companyName.isEmpty() ? "Your Company" : companyName;
    QString lProductName = productName.isEmpty() ? "Your Product" : productName;

    ProductId = "-//" + lCompanyName + "//NONSGML " + lProductName + "//EN";
    icalcomponent_add_property(cal, icalproperty_new_prodid(ProductId.toStdString().c_str()));

    icalcomponent *event = icalcomponent_new(ICAL_VEVENT_COMPONENT);
    if (!event) {
        Logger::LogMessage("Error: Failed to create VEVENT component.");
        icalcomponent_free(cal);
        return QString();
    }

    // Check if end time is after start time
    icaltimetype start_time = icaltime_from_string(start.toUtf8().data());
    icaltimetype end_time = icaltime_from_string(end.toUtf8().data());

    if (icaltime_compare(start_time, end_time) > 0) {
        Logger::LogMessage("Error: End time is before start time. Setting end time to start time.");
        end_time = start_time;
    }

    icalcomponent_add_property(event, icalproperty_new_summary(summary.toUtf8().data()));
    icalcomponent_add_property(event, icalproperty_new_dtstart(start_time));
    icalcomponent_add_property(event, icalproperty_new_dtend(end_time));

    icalcomponent_add_component(cal, event);

    char *ical_string = icalcomponent_as_ical_string(cal);
    if (!ical_string) {
        Logger::LogMessage("Error: Failed to generate iCalendar string.");
        icalcomponent_free(cal);
        return QString();
    }

    QString icsContent = QString::fromUtf8(ical_string);
    free(ical_string);
    icalcomponent_free(cal);

    return icsContent;
}

void IcalBuilder::Initialize()
{
    eventName = "Unspecified Meeting";
    startTime = QDateTime::currentDateTime();
    endTime = startTime.addSecs(1800); //default to 30 minutes
    companyName = "Unspecified Company";
    productName = "Unspecified Product";
}

} // namespace Nutmeg
