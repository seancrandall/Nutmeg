#include "optionswindow.h"

namespace Nutmeg
{

OptionsWindow::OptionsWindow(Settings *settings, QWidget *parent) : Dialog(parent), mSettings(settings)
{
    optionsDatabasePanel = new OptionsDatabasePanel(this);
    optionsDocumentsPanel = new OptionsDocumentPanel(this);
    optionsDisplayPanel = new OptionsDisplayPanel(this);

    tabber = new QTabWidget(this);
    tabber->addTab(optionsDatabasePanel, "Database");
    tabber->addTab(optionsDocumentsPanel, "Documents");
    tabber->addTab(optionsDisplayPanel, "Display");

    OptionsWindow::setupDisplay();
    OptionsWindow::connectSignalsAndSlots();
    OptionsWindow::slotScatter();
}

void OptionsWindow::slotScatter()
{
    optionsDatabasePanel->server = mSettings->server;
    optionsDatabasePanel->port = mSettings->port;
    optionsDatabasePanel->databaseName = mSettings->databaseName;
    optionsDatabasePanel->username = mSettings->username;
    optionsDatabasePanel->password = mSettings->password;

    optionsDocumentsPanel->documentsLocation = mSettings->docsLocation;
    optionsDocumentsPanel->maxFilenameLength = mSettings->maxFilenameLength;

    optionsDisplayPanel->fontFamily = mSettings->fontFamily;
    optionsDisplayPanel->fontSize = mSettings->fontSize;
}

void OptionsWindow::slotGather()
{
    mSettings->server = optionsDatabasePanel->server;
    mSettings->port = optionsDatabasePanel->port;
    mSettings->databaseName = optionsDatabasePanel->databaseName;
    mSettings->username = optionsDatabasePanel->username;
    mSettings->password = optionsDatabasePanel->password;

    mSettings->docsLocation = optionsDocumentsPanel->documentsLocation;
    mSettings->maxFilenameLength = optionsDocumentsPanel->maxFilenameLength;

    mSettings->fontFamily = optionsDisplayPanel->fontFamily;
    mSettings->fontSize = optionsDisplayPanel->fontSize;
}

void OptionsWindow::slotHandleRejected() { Dialog::slotHandleRejected(); }

void OptionsWindow::slotHandleAccepted()
{
    slotGather();
    Dialog::slotHandleAccepted();
}

void OptionsWindow::setupDisplay()
{
    setWindowTitle("Options");
    workLayout->addWidget(tabber, 0, 0);
    updateLayout();
}

void OptionsWindow::connectSignalsAndSlots() { Dialog::connectSignalsAndSlots(); }

} // namespace Nutmeg
