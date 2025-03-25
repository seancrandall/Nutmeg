#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QStandardPaths>
#include <QCoreApplication>

namespace Nutmeg {

class Logger {
  public:
    // Logs a message with a timestamp and appends it to the log file
    static const QString LogMessage(const QString& message);

           // Initializes the log line count by counting existing lines in the log file
    static unsigned int InitializeLogCount();

    static unsigned int maxLogLines; /// The maximum number of log lines to keep. Once this number is reached, the log file will be trimmed to preferredLogLines.
    static unsigned int preferredLogLines; ///The "preferred" number of lines. The log will be allowed to grow to maxLogLines, but then will be trimmed to preferredLogLines. This prevents rotating the log every time preferredLogLines is exceeded.

  private:
    // Rotates the log file if it exceeds the maximum number of lines
    static void rotateLogFile(const QString& filePath, unsigned int maxLines, unsigned int preferredLines);

           // Returns the full path to the log file
    static QString getLogFilePath();

           // Static member to maintain the log line count
    static unsigned int logLineCount;
};

} // namespace Nutmeg

#endif // LOGGER_H
