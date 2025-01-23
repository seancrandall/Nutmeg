#include "nutmeg.h"

namespace Nutmeg
{

//#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Wunused-function"

QStringList inferName(QString entityName)
{
    // Split the entityName using space as the delimiter
    QStringList parts = entityName.split(' ', Qt::SkipEmptyParts);

    // Handle cases where there's no space or only one part
    if (parts.size() < 2)
    {
        // If there's no space or only one word, assume the entire name is either
        // first or last name
        return QStringList() << parts.value(0) << QString();
    }

    // The first name is the first word
    QString firstName = parts.first();

    // The last name is the part after the last space (join the remaining words)
    QString lastName = parts.mid(1).join(' ');

    return QStringList() << firstName << lastName;
}

QString inferEnterpriseName(QString firstName, QString lastName) { return firstName + " " + lastName; }

//#pragma clang diagnostic pop

} // namespace Nutmeg
