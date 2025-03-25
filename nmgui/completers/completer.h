#ifndef NUTMEG_COMPLETER_H
#define NUTMEG_COMPLETER_H

#include <QCompleter>
#include <QObject>

namespace Nutmeg
{

class Completer : public QCompleter
{
    Q_OBJECT
  public:
    Completer(QObject *parent = nullptr);
    Completer(QAbstractItemModel *model, QObject *parent = nullptr);

    void InitializeCompleter(void);

  signals:
    void signalModelAvailable(QAbstractTableModel *availableModel);
  public slots:
    virtual void slotUpdateModel(QAbstractTableModel *newmodel);
};

} // namespace Nutmeg

#endif // NUTMEG_COMPLETER_H
