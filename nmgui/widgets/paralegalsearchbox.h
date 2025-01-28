#ifndef PARALEGALSEARCHBOX_H
#define PARALEGALSEARCHBOX_H

#include <QObject>
#include <QWidget>

#include "personsearchbox.h"
#include "dbaccess/viewparalegalsmodel.h"

namespace Nutmeg {

class ParalegalSearchBox : public Nutmeg::PersonSearchBox
{
    Q_OBJECT
public:
    ParalegalSearchBox(QWidget *parent = nullptr);
    ParalegalSearchBox(Key inikey, QWidget *parent = nullptr);

private:
    viewParalegalsModel *paraModel = new viewParalegalsModel(this);

    void Initialize(void);
};

} // namespace Nutmeg

#endif // PARALEGALSEARCHBOX_H
