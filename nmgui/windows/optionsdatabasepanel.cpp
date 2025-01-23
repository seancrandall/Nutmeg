#include "optionsdatabasepanel.h"

namespace Nutmeg
{

OptionsDatabasePanel::OptionsDatabasePanel(QWidget *parent) : QWidget{parent}
{
    leServer = new LineEdit();
    lePort = new LineEdit();
    leDatabaseName = new LineEdit();
    leUsername = new LineEdit();
    lePassword = new LineEdit();

    wServer = new LabeledWidgetLeft("Server", leServer);
    wPort = new LabeledWidgetLeft("Port", lePort);
    wDatabaseName = new LabeledWidgetLeft("Database Name", leDatabaseName);
    wUsername = new LabeledWidgetLeft("Username", leUsername);
    wPassword = new LabeledWidgetLeft("Password", lePassword);

    vlayout = new QVBoxLayout(this);

    vlayout->addLayout(wServer);
    vlayout->addLayout(wPort);
    vlayout->addLayout(wDatabaseName);
    vlayout->addLayout(wUsername);
    vlayout->addLayout(wPassword);
    vlayout->addStretch();
    adjustSize();
}

} // namespace Nutmeg
