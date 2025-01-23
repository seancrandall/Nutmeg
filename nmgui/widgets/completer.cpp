#include "completer.h"

namespace Nutmeg
{

Completer::Completer(QObject *parent) : QCompleter(parent) { InitializeCompleter(); }

Completer::Completer(QAbstractItemModel *model, QObject *parent) : QCompleter(model, parent) { InitializeCompleter(); }

void Completer::InitializeCompleter()
{
    // setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    setCompletionMode(QCompleter::PopupCompletion);
    setCompletionRole(Qt::EditRole);
    setFilterMode(Qt::MatchContains);
    setMaxVisibleItems(10);
    setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    setCaseSensitivity(Qt::CaseInsensitive);
}

void Completer::slotUpdateModel(QAbstractTableModel *newmodel)
{
    setModel(newmodel);
    emit signalModelAvailable(newmodel);
}

} // namespace Nutmeg
