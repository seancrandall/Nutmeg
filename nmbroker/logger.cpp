#include <QDir>
#include <QFileInfo>
#include <QCoreApplication>

#include "logger.h"

namespace Nutmeg {

// Initialize the static log line count
unsigned int Logger::logLineCount = 0;
unsigned int Logger::maxLogLines = 1500;
unsigned int Logger::preferredLogLines = 1000;

const QString Logger::LogMessage(const QString& message) {
    // Create a time-stamped message
    QString timeStampedMessage = QString();
    timeStampedMessage += QString("[%1] %2")
                            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm"))
                            .arg(message);

    // Also print to console immediately for operator feedback
    qInfo() << timeStampedMessage;
           // Get the log file path
    QString logFilePath = getLogFilePath();

           // Ensure the log file doesn't exceed max lines
    rotateLogFile(logFilePath, maxLogLines, preferredLogLines);

           // Open the log file in append mode
    QFile logFile(logFilePath);
    if (!logFile.open(QIODevice::Append | QIODevice::Text)) {
        qWarning() << "Failed to open log file:" << logFilePath;
        return timeStampedMessage; // Return the message even if logging fails
    }

           // Append the message to the file
    QTextStream out(&logFile);
    out << timeStampedMessage << '\n';
    logFile.close();

           // Increment the line count
    ++logLineCount;

    return timeStampedMessage;
}

unsigned int Logger::InitializeLogCount() {
    QString logFilePath = getLogFilePath();
    QFile logFile(logFilePath);

    if (!logFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open log file for reading:" << logFilePath;
        logLineCount = 0;
        return logLineCount;
    }

           // Count the number of lines in the file
    QTextStream in(&logFile);
    logLineCount = 0;
    while (!in.atEnd()) {
        in.readLine();
        ++logLineCount;
    }
    logFile.close();

    return logLineCount;
}

void Logger::rotateLogFile(const QString& filePath, unsigned int maxLines, unsigned int preferredLines) {
    if (logLineCount <= maxLines) {
        return; // No need to rotate
    }

    QFile logFile(filePath);
    if (!logFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open log file for rotation:" << filePath;
        return;
    }

           // Read only the last `preferredLines` lines
    QStringList lines;
    QTextStream in(&logFile);
    while (!in.atEnd()) {
        lines.append(in.readLine());
        if (lines.size() > preferredLines) {
            lines.removeFirst(); // Keep only the last `preferredLines` lines
        }
    }
    logFile.close();

           // Write the trimmed lines back to the file
    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open log file for writing:" << filePath;
        return;
    }

    QTextStream out(&logFile);
    for (const QString& line : lines) {
        out << line << '\n';
    }
    logFile.close();

           // Update the line count to the trimmed size
    logLineCount = lines.size();
}

QString Logger::getLogFilePath() {
    // Get the application's standard data location
    QString appDataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(appDataLocation);

           // Ensure the directory exists
    if (!dir.exists() && !dir.mkpath(".")) {
        qWarning() << "Failed to create log directory:" << appDataLocation;
    }

           // Return the full path to the log file
    QString appName = QCoreApplication::applicationName();
    return dir.filePath(appName + ".log");
}

} // namespace Nutmeg
