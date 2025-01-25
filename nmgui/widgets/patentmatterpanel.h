#ifndef PATENTMATTERPANEL_H
#define PATENTMATTERPANEL_H

#include <QObject>
#include <QWidget>
#include "frame.h"

namespace Nutmeg {

class PatentMatterPanel : public Nutmeg::Frame
{
    Q_OBJECT
public:
    PatentMatterPanel();
};

} // namespace Nutmeg

#endif // PATENTMATTERPANEL_H
