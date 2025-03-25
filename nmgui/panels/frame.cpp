#include "frame.h"

namespace Nutmeg{

Frame::Frame(QWidget *parent)
    : QFrame(parent)
{
    //setFrameStyle(QFrame::StyledPanel);
    setFrameStyle(QFrame::NoFrame);
}

}
