#ifndef MAINDASHBOARD_H
#define MAINDASHBOARD_H

#include <QSplitter>
#include <QObject>
#include <QWidget>

#include "frame.h"

namespace Nutmeg {

class MainDashboard : public QSplitter
{
    Q_OBJECT
public:
    MainDashboard(QWidget *parent=nullptr);
};

} // namespace Nutmeg

#endif // MAINDASHBOARD_H
