#ifndef FRAME_H
#define FRAME_H

#include <QFrame>
#include <QWidget>

namespace Nutmeg
{

class Frame : public QFrame
{
    Q_OBJECT
  public:
    Frame(QWidget *parent=nullptr);
};

} //end namespace

#endif // FRAME_H
