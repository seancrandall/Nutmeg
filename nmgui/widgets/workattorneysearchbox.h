#ifndef NUTMEG_WORKATTORNEYSEARCHBOX_H
#define NUTMEG_WORKATTORNEYSEARCHBOX_H

#include <QObject>
#include <QWidget>

#include "personsearchbox.h"
#include "dbaccess/viewworkattorneysmodel.h"

namespace Nutmeg
{

class WorkAttorneySearchBox : public Nutmeg::PersonSearchBox
{
    Q_OBJECT
  public:
    WorkAttorneySearchBox(QWidget *parent = nullptr);
    WorkAttorneySearchBox(Key initkey, QWidget *parent = nullptr);

private:
    viewWorkAttorneysModel *attyModel;

    void Initialize(void);
    void CheckModels(void);
};

} // namespace Nutmeg

#endif // NUTMEG_WORKATTORNEYSEARCHBOX_H
