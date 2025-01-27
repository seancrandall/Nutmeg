#ifndef OPTIONSDATABASEPANEL_H
#define OPTIONSDATABASEPANEL_H

#include <QObject>
#include <QWidget>

#include "property.h"
#include "widgets/labeledwidgetleft.h"
#include "widgets/lineedit.h"

namespace Nutmeg
{

class OptionsDatabasePanel : public QWidget
{
    Q_OBJECT
  public:
    explicit OptionsDatabasePanel(QWidget *parent = nullptr);

    // Public Properties
    Property(getServer, setServer) QString server;
    Property(getPort, setPort) uint port;
    Property(getDatabaseName, setDatabaseName) QString databaseName;
    Property(getUsername, setUsername) QString username;
    Property(getPassword, setPassword) QString password;

    const QString getServer(void) const { return leServer->text; };

    uint getPort(void) const { return lePort->text.toUInt(); };

    const QString getDatabaseName(void) const { return leDatabaseName->text; };

    const QString getUsername(void) const { return leUsername->text; };

    const QString getPassword(void) const { return lePassword->text; };

    void setServer(const QString newserver) { leServer->text = newserver; };

    void setPort(const uint newport) { lePort->text = QString::number(newport); };

    void setDatabaseName(const QString newDatabaseName) { leDatabaseName->text = newDatabaseName; };

    void setUsername(const QString newUsername) { leUsername->text = newUsername; };

    void setPassword(const QString newPassword) { lePassword->text = newPassword; };

  private:
    LineEdit *leServer;
    LineEdit *lePort;
    LineEdit *leDatabaseName;
    LineEdit *leUsername;
    LineEdit *lePassword;

    LabeledWidgetLeft *wServer;
    LabeledWidgetLeft *wPort;
    LabeledWidgetLeft *wDatabaseName;
    LabeledWidgetLeft *wUsername;
    LabeledWidgetLeft *wPassword;

    QVBoxLayout *vlayout;

  signals:
};

} // namespace Nutmeg

#endif // OPTIONSDATABASEPANEL_H
