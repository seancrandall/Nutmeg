#include <QUrl>
#include <QUrlQuery>
#include <QDesktopServices>
#include <QStandardPaths>
#include <QTextStream>
#include <QFile>
#include <QIODevice>
#include <QDir>
#include <QUuid>
#include <QDebug>

#include "email.h"

namespace Nutmeg {

Email::Email()
{
    Initialize();
}

void Email::setFrom(const QString &newfrom)
{
    mFrom = newfrom;
}

const QString &Email::getFrom() const
{
    return mFrom;
}

void Email::setTo(const QList<QString> &newlist)
{
    mTo = newlist;
}

const QList<QString> &Email::getTo() const
{
    return mTo;
}

void Email::addRecipient(const QString &newRecipient)
{
    mTo << newRecipient;
}

const QList<QString> &Email::getCCList() const
{
    return mCC;
}

void Email::setCCList(const QList<QString> &newCCList)
{
    mCC = newCCList;
}

void Email::addCC(const QString &newCC)
{
    mCC <<  newCC;
}

const QList<QString> &Email::getBCCList() const
{
    return mBCC;
}

void Email::setBCCList(const QList<QString> &newBCCList)
{
    mBCC = newBCCList;
}

void Email::addBCC(const QString &newBCC)
{
    mBCC << newBCC;
}

const QString &Email::getSubject() const
{
    return mSubject;
}

void Email::setSubject(const QString &newSubject)
{
    mSubject = newSubject;
}

const QString &Email::getBody() const
{
    return mBody;
}

void Email::setBody(const QString &newBody)
{
    mBody = newBody;
}

const QString &Email::getEmlString()
{
    mEmlString = BuildEmlString();
    return mEmlString;
}


void Email::open()
{
    QUrl mailtoUrl("mailto:");

    QUrlQuery query;

    // Add 'to' recipients
    if (!mTo.isEmpty()) {
        query.addQueryItem("to", mTo.join(","));
    }

    // Add 'cc' recipients
    if (!mCC.isEmpty()) {
        query.addQueryItem("cc", mCC.join(","));
    }

    // Add 'bcc' recipients
    if (!mBCC.isEmpty()) {
        query.addQueryItem("bcc", mBCC.join(","));
    }

    // Add subject
    if (!mSubject.isEmpty()) {
        query.addQueryItem("subject", mSubject);
    }

    // Add body
    if (!mBody.isEmpty()) {
        query.addQueryItem("body", mBody);
    }

    // Set the query part of the URL
    mailtoUrl.setQuery(query);

    // Open the URL with the default email client
    if (!QDesktopServices::openUrl(mailtoUrl)) {
        // Handle error, like logging it or showing a message to the user
        qDebug() << "Failed to open email client with URL: " << mailtoUrl.toString();
    }
}

void Email::Initialize()
{
    mFrom = QString();
    mTo = QList<QString>();
    mCC = QList<QString>();
    mBCC = QList<QString>();
    mSubject = QString();
    mBody = QString();
    mEmlString = QString();
}

QString Email::BuildEmlString()
{
    QString eml;
    // From header
    eml += "From: " + mFrom + "\r\n";

    // To header
    if (!mTo.isEmpty()) {
        eml += "To: " + mTo.join(", ") + "\r\n";
    }

    // CC header
    if (!mCC.isEmpty()) {
        eml += "Cc: " + mCC.join(", ") + "\r\n"; // Note: 'Cc' is capitalized per RFC 5322
    }

    // Bcc header
    if (!mBCC.isEmpty()) {
        eml += "Bcc: " + mBCC.join(", ") + "\r\n"; // 'Bcc' is also capitalized
    }

    // Subject header
    eml += "Subject: " + mSubject + "\r\n\r\n";

    // Body
    eml += mBody;

    return eml;
}

} // namespace Nutmeg
