#ifndef EMAIL_H
#define EMAIL_H

#include <QString>
#include <QList>
#include "property.h"

namespace Nutmeg {

class Email
{
public:
    Email();
    ~Email() = default;

    Property(getFrom, setFrom) QString from;
    Property(getToList, setToList) QList<QString> to;
    Property(getCCList, setCCList) QList<QString> cc;
    Property(getBCCList, setBCCList) QList<QString> bcc;
    Property(getSubject, setSubject) QString subject;
    Property(getBody, setBody) QString body;
    ReadOnlyProperty(getEmlString) QString emlString;

    void setFrom(const QString& newfrom);
    const QString& getFrom(void) const;

    void setTo(const QList<QString>& newlist);
    const QList<QString>& getTo(void) const;
    void addRecipient(const QString& newRecipient);

    const QList<QString>& getCCList(void) const;
    void setCCList(const QList<QString>& newCCList);
    void addCC(const QString& newCC);

    const QList<QString>& getBCCList(void) const;
    void setBCCList(const QList<QString>& newBCCList);
    void addBCC(const QString& newBCC);

    const QString& getSubject(void) const;
    void setSubject(const QString& newSubject);

    const QString& getBody(void) const;
    void setBody(const QString& newBody);

    const QString& getEmlString(void);

    void open(void);

private:
    QString mFrom;
    QList<QString> mTo;
    QList<QString> mCC;
    QList<QString> mBCC;
    QString mSubject;
    QString mBody;

    QString mEmlString;

    void Initialize(void);
    QString BuildEmlString(void);
};

} // namespace Nutmeg

#endif // EMAIL_H
