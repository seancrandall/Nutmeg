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

const QString IcalBuilder::getDescription() const
{
    return mDescription;
}

void IcalBuilder::setDescription(const QString &description)
{
    mDescription = description;
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

const QString IcalBuilder::getOutputLocation()
{
    return mOutputLocation;
}

bool IcalBuilder::setOutputLocation(const QString &newloc)
{
    mOutputValid = false;
    // Convert the path to a QFileInfo object to check its properties
    QFileInfo fileInfo(newloc);

    // Check if the path exists
    if (!fileInfo.exists()) {
        Logger::LogMessage(QString("Error: Output location '%1' does not exist.").arg(newloc));
        return false;
    }

    // Check if the path is a directory
    if (!fileInfo.isDir()) {
        Logger::LogMessage(QString("Error: Output location '%1' is not a directory.").arg(newloc));
        return false;
    }

    // Check if the directory is writable
    if (!fileInfo.isWritable()) {
        Logger::LogMessage(QString("Error: Output location '%1' is not writable.").arg(newloc));
        return false;
    }

    // If all checks pass, set the new location
    mOutputLocation = newloc;
    mOutputValid = true;
    return true;
}

bool IcalBuilder::openIcsFile()
{
    QString icsString = getIcsString();
    if (icsString.isEmpty()) {
        Logger::LogMessage("Error: No ICS string to write to file.");
        return false;
    }

    if(!mOutputValid){
        Logger::LogMessage(QString("Could not make an ICS file because %1 is not writeable").arg(mOutputLocation));
        return false;
    }

    // Create a unique file name
    QString fileName = outputLocation + QDir::separator() + "event_" + QUuid::createUuid().toString() + ".ics";

    // Write out the ICS file
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << icsString;
        file.close();
        Logger::LogMessage(QString("Wrote ICS file %1.").arg(file.fileName()));

        // Use QDesktopServices to open the ICS file with the default handler
        if (!QDesktopServices::openUrl(QUrl::fromLocalFile(fileName))) {
            // Handle error - file could not be opened by default application
            Logger::LogMessage("Error: Failed to open ICS file with default application.");
            return false;
        }
    } else {
        // Handle error - file could not be created
        Logger::LogMessage("Error: Failed to create ICS file at: " + fileName);
        return true;
    }
    return true;
}

QString IcalBuilder::getIcsString()
{
    if (mEventName.isEmpty() || mStartTimeString.isEmpty() || mEndTimeString.isEmpty()) {
        Logger::LogMessage("Error: Cannot generate ICS string. Missing necessary data.");
        return QString();
    }

    QString icsString;
    QTextStream stream(&icsString, QIODevice::WriteOnly);

    // VCALENDAR component
    stream << "BEGIN:VCALENDAR\n";
    stream << "VERSION:2.0\n";
    stream << "PRODID:-//" << mCompanyName << "//NONSGML " << mProductName << "//EN\n";

    // VEVENT component
    stream << "BEGIN:VEVENT\n";
    stream << "SUMMARY:" << mEventName << "\n";
    stream << "DTSTART:" << mStartTimeString << "\n";
    stream << "DTEND:" << mEndTimeString << "\n";

    if (!mDescription.isEmpty()) {
        // Handle multi-line description by folding lines at 75 characters
        QStringList lines = mDescription.split('\n');
        stream << "DESCRIPTION:";
        for (int i = 0; i < lines.size(); i++) {
            if (i > 0) {
                stream << " "; // Space for continuation
            }
            QString line = lines[i];
            while (!line.isEmpty()) {
                // Fold long lines
                QString segment = line.left(75);
                stream << segment << "\n";
                line = line.mid(75);
                if (!line.isEmpty()) {
                    stream << " "; // Space for continuation of folded line
                }
            }
        }
    }

    //Insert a newline so that there's not problem with the description
    stream << "\n";

    // Add reminder if set
    if (mReminderMinutes > 0) {
        stream << "BEGIN:VALARM\n";
        stream << "ACTION:DISPLAY\n";
        stream << "DESCRIPTION:Reminder: Event starts soon\n";
        stream << "TRIGGER:-PT" << mReminderMinutes << "M\n";
        stream << "END:VALARM\n";
    }

    stream << "END:VEVENT\n";
    stream << "END:VCALENDAR\n";

    return icsString;
}

void IcalBuilder::Initialize()
{
    eventName = "Unspecified Meeting";
    startTime = QDateTime::currentDateTime();
    endTime = startTime.addSecs(1800); //default to 30 minutes
    companyName = "Unspecified Company";
    productName = "Unspecified Product";

    // Use QStandardPaths to get a temporary location
    QString tempDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    if (tempDir.isEmpty()) {
        Logger::LogMessage("Error: Could not find a writable temporary directory for ICS file.");
    } else {
        outputLocation = tempDir;
        mOutputValid = true;
    }
}

} // namespace Nutmeg
