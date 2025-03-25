#ifndef NUTMEG_FILINGSVIEW_H
#define NUTMEG_FILINGSVIEW_H

#include "tableview.h"
#include <QObject>
#include <QWidget>

namespace Nutmeg
{

class FilingsView : public Nutmeg::TableView
{
    Q_OBJECT
  public:
    FilingsView(QWidget *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_FILINGSVIEW_H
