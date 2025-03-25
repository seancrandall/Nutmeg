#ifndef ICALBUILDER_H
#define ICALBUILDER_H

#include <QString>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QUrl>
#include <QDir>
#include <QUuid>
#include <QDesktopServices>
#include <QStandardPaths>

#include "property.h"
#include "settings.h"

namespace Nutmeg {

class IcalBuilder
{
public:
    IcalBuilder(void);
    IcalBuilder(const QDateTime& apptTime, QDateTime apptEnd=QDateTime());
    ~IcalBuilder();

    Property(getEventName, setEventName) QString eventName;
    Property(getStartTime, setStartTime) QDateTime startTime;
    Property(getDescription, setDescription) QString description;
    Property(getEndTime, setEndTime) QDateTime endTime;
    Property(getCompanyName, setCompanyName) QString companyName;
    Property(getProductName, setProductName) QString productName;
    Property(getOutputLocation, setOutputLocation) QString outputLocation;
    ReadOnlyProperty(getOutputValid) bool outputValid;
    ReadOnlyProperty(getIcsString) QString icsString;
    Property(getReminderMinutes, setReminderMinutes) uint reminderMinutes;

    const QString getEventName(void) const;
    void setEventName(const QString &newname);
    const QDateTime getStartTime(void) const;
    void setStartTime(const QDateTime &newtime);
    const QDateTime getEndTime(void) const;
    void setEndTime(const QDateTime &newtime);
    const QString getDescription(void) const;
    void setDescription(const QString& description);
    const QString getCompanyName(void) const;
    void setCompanyName(const QString& newname);
    const QString getProductName(void) const;
    void setProductName(const QString& newname);
    const QString getOutputLocation(void);
    bool setOutputLocation(const QString& newloc);
    bool getOutputValid(void) {return mOutputValid;}
    uint getReminderMinutes(void) const;
    void setReminderMinutes(uint mins);
    QString getIcsString(void);

    bool openIcsFile(void);

private:
    QString mEventName;
    QDateTime mStartTime;
    QString mStartTimeString;
    QDateTime mEndTime;
    QString mEndTimeString;
    QString mDescription;
    QString mCompanyName;
    QString mProductName;
    QString mIcsFileString;
    QString mOutputLocation;
    bool mOutputValid = false;
    uint mReminderMinutes = 15;

    void Initialize(void);
};

} // namespace Nutmeg

#endif // ICALBUILDER_H
