#ifndef NUTMEG_H
#define NUTMEG_H

#include <QDate>
#include <QDateTime>
#include <QHash>
#include <QList>
#include <QMap>
#include <QStringList>
#include <QVariant>
#include <QVariantList>
#include <QVector>
//#include <boost/date_time.hpp>
//#include <boost/date_time/gregorian_calendar.hpp>
//#include <boost/date_time/posix_time/posix_time.hpp>
//#include <boost/static_string.hpp>

// #include "property.h"

#define TRY(x)                                                                                                         \
    try                                                                                                                \
    {                                                                                                                  \
        x                                                                                                              \
    }                                                                                                                  \
    catch (...)                                                                                                        \
    {                                                                                                                  \
        throw;                                                                                                         \
    }

#define NUTMEGFONT (setFont(QFont(FontFamily, FontSize)))

#define PU(x) std::unique_ptr<(x)>
#define PS(x) std::shared_ptr<(x)>

namespace Nutmeg
{
typedef QString String;
typedef uint32_t Key;
typedef uint32_t Uint32;
typedef uint64_t Uint64;
typedef uint8_t Uint8;
typedef std::vector<uint8_t> ByteVector;
// typedef std::chrono::year_month_day Date;
typedef QDate Date;
typedef QDateTime DateTime;
#define Vector QVector
#define UniquePtr std::uniqe_ptr
#define Cerror std::cerr

// Global Variables
static String FontFamily;
static int FontSize;

// Static Functions
//#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Wunused-function"
QStringList inferName(QString entityName);
QString inferEnterpriseName(QString firstName, QString lastName);
//#pragma clang diagnostic pop

} // namespace Nutmeg

#endif // NUTMEG_H
